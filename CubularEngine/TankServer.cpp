#include "stdafx.h"
#include "TankServer.h"
#include "GameTime.h"

using namespace Networking;

TankServer::TankServer( const int aPort )
    : Port( aPort )
{
    DEBUG_PRINT( "--- Server App ---" );
    DEBUG_PRINT( "Running on Port: %d", Port );
    timer = GameTime::GetInstance();
    timer->Init();

    ServerThread = std::thread( &Networking::TankServer::ServerWorker, this );
    UpdateGameObjects();
}

TankServer::~TankServer()
{
    isDone = true;
    // Cleanup -------------------
    closesocket( ServerSocket );
    WSACleanup();
}

void Networking::TankServer::ShutDown()
{
    isDone = true;

    if ( ServerThread.joinable() )
    {
        ServerThread.join();
    }
}

void TankServer::ServerWorker()
{
    // Init winsoc ---------------------
    WSADATA wsaData;

    struct sockaddr_in server, si_other;
    int slen, recv_len;
    char buf[ DEF_BUF_LEN ];
    char ip_str[ INET_ADDRSTRLEN ];
    slen = sizeof( si_other );

    DEBUG_PRINT( "Initializing winsock..." );

    if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 )
    {
        fprintf( stderr, "FAILED. Error code : %d ", WSAGetLastError() );
        exit( EXIT_FAILURE );
    }

    DEBUG_PRINT( "Initialized winsock!" );

    // Create listen socket -----------------------
    if ( ( ServerSocket = socket( AF_INET, SOCK_DGRAM, 0 ) ) == SOCKET_ERROR )
    {
        DEBUG_PRINT( "Could not create socket : %d ", WSAGetLastError() );
    }

    DEBUG_PRINT( "Socket created." );

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( Port );

    if ( bind( ServerSocket, ( struct sockaddr* )&server, sizeof( server ) ) == SOCKET_ERROR )
    {
        DEBUG_PRINT( "Could not create socket : %d ", WSAGetLastError() );
        exit( EXIT_FAILURE );
    }

    DEBUG_PRINT( "Bind done." );

    // Wait for data ------------------------------
    while ( 1 )
    {
        if ( isDone ) return;

        DEBUG_PRINT( "Waiting for data..." );
        fflush( stdout );

        memset( buf, '\0', DEF_BUF_LEN );

        if ( ( recv_len = recvfrom( ServerSocket, buf, DEF_BUF_LEN, 0, ( struct sockaddr* ) &si_other, &slen ) ) == SOCKET_ERROR )
        {
            DEBUG_PRINT( "recvfrom failed with error code : %d", WSAGetLastError() );
            exit( EXIT_FAILURE );
        }

        // Get the IP of this data and see if the client exists
        memset( ip_str, '\0', INET_ADDRSTRLEN );
        inet_ntop( AF_INET, &( si_other.sin_addr ), ip_str, INET_ADDRSTRLEN );
        std::string newIP = ip_str;

        DEBUG_PRINT( "Data received %s from %s", buf, ip_str );

        if ( !ClientExists( newIP ) )
        {
            char ackCmd[ 64 ];

            //put conenction type in the first thing
            int32_t type = ConnectionType::ConnectionAck;
            memcpy( ackCmd, &type, sizeof( int32_t ) );

            //assign the client ID and put it in the command and reply to the client
            Command outCommand = { };
            outCommand.clientId = ConnectedClients.size();
            memcpy( ackCmd + sizeof( int32_t ), &outCommand, sizeof( Command ) );

            ConnectedClients.push_back( newIP );
            BroadcastedGameObject gObj = { };
            gObj.gameObjId = outCommand.clientId;


            // Send an acknowledgment back to this client that they have connected
            if ( sendto( ServerSocket, ackCmd, sizeof( Command ) + sizeof( int32_t ), 0, ( struct sockaddr * )&si_other, slen ) == SOCKET_ERROR )
            {
                DEBUG_PRINT( "ack send failed with error code : %d", WSAGetLastError() );
                exit( EXIT_FAILURE );
            }

            gameObjects.push_back( gObj );
        }
        else  //otherwise process the command and reply with broadcast stuff
        {
            ProcessCmd( buf );

            memset( buf, '\0', DEF_BUF_LEN );
            std::vector<BroadcastedGameObject> broadcastedObjs = std::vector<BroadcastedGameObject>();
            int32_t connectionType = ConnectionType::ConnectionAck; //default (won't get processed)

            if ( broadcastedObject.size() > 0 )
            {
                broadcastedObject.pop_front( broadcastedObjs );
                
                //set the connection type
                if ( broadcastedObjs.size() > 0 )
                    connectionType = ConnectionType::Broadcast;
            }

            memcpy( buf, &connectionType, sizeof( int32_t ) );

            //set the count
            int32_t count = static_cast<int32_t>( broadcastedObjs.size() );
            memcpy( buf + sizeof( int32_t ), &count, sizeof( int32_t ) );

            //copy the rest of the broadcasted objects over
            if ( broadcastedObjs.size() > 0 )
                memcpy( buf + 2 * sizeof( int32_t ), &broadcastedObjs[ 0 ], sizeof( BroadcastedGameObject ) * count );

            // Do something with the received message
            if ( sendto( ServerSocket, buf, sizeof( BroadcastedGameObject ) * count + 2 * sizeof( int32_t ), 0, ( struct sockaddr * )&si_other, slen ) == SOCKET_ERROR )
            {
                DEBUG_PRINT( "sendto failed with error code : %d", WSAGetLastError() );
                exit( EXIT_FAILURE );
            }
        }
    }
}

inline const int Networking::TankServer::GetPort() const
{
    return Port;
}

void TankServer::UpdateGameObjects()
{
    while ( 1 )
    {
        if ( isDone ) return;

        //basically pop out a bunch of commands from the queue and fill out the vector
        size_t cmdCount = commandQueue.size();
        std::vector<Command> cmdVector( cmdCount );
        for ( int i = 0; i < cmdCount; ++i )
        {
            Command c = {};
            commandQueue.pop_front( c );
            cmdVector[ i ] = c;
        }

        //loop through the vector of command
        for ( size_t i = 0; i < cmdVector.size(); ++i )
        {
            Command cmd = cmdVector[ i ];

            //apply the right commands to the right game object in an inefficiant O(n2) way
            for ( size_t j = 0; j < gameObjects.size(); ++j )
            {
                if ( cmd.clientId == gameObjects[ j ].gameObjId )
                {

                    //process the command
                    if ( cmd.move_down )
                    {
                        gameObjects[ j ].y--;
                    }
                    else if ( cmd.move_up )
                    {
                        gameObjects[ j ].y++;
                    }
                    else if ( cmd.move_left )
                    {
                        gameObjects[ j ].x++;
                    }
                    else if ( cmd.move_right )
                    {
                        gameObjects[ j ].x--;
                    }
                }
            }
        }

        //emplace a copy of the gameobj state
        if ( cmdCount > 0 )
        {
            std::vector< BroadcastedGameObject > toBroadcast( gameObjects.size() );
            memcpy( &toBroadcast[ 0 ], &gameObjects[ 0 ], gameObjects.size() * sizeof( BroadcastedGameObject ) );
            broadcastedObject.emplace_back( toBroadcast );
        }
    }
}

bool TankServer::ClientExists( std::string & clientIP )
{
    auto itr = ConnectedClients.begin();
    for ( ; itr != ConnectedClients.end(); ++itr )
    {
        if ( *( itr ) == clientIP )
            return true;
    }

    return false;
}

void TankServer::ProcessCmd( char * aCmd )
{
    assert( aCmd != nullptr );

    Command outCommand;
    memcpy( &outCommand, aCmd, sizeof( Command ) );

    //validate the packet

    if ( outCommand.move_left && outCommand.move_right )
    {
        outCommand.move_left = 0;
        outCommand.move_right = 0;
    }

    if ( outCommand.move_down && outCommand.move_up )
    {
        outCommand.move_down = 0;
        outCommand.move_up = 0;
    }

    commandQueue.emplace_back( outCommand );
}

void TankServer::BroadCastToAllClients()
{

}

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
    gameObjects = std::vector<BroadcastedGameObject>();
    gameObjects.reserve( MAX_CONNECTIONS );

    ServerWorker();
}

TankServer::~TankServer()
{
    isDone = true;
    // Cleanup -------------------
    closesocket( ServerSocket );
    WSACleanup();
}

void TankServer::ShutDown()
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

        DEBUG_PRINT( "Data received from %s", ip_str );

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

            DEBUG_PRINT( "First time receiving command from client ID %d", gObj.gameObjId );

            //add the gameobject to the vector of gameobjects if 
            bool contains = false;
            for ( size_t i = 0; i < gameObjects.size(); ++i )
            {
                if ( gameObjects[ i ].gameObjId == gObj.gameObjId )
                {
                    contains = true;
                    break;
                }
            }

            if ( !contains )
                gameObjects.push_back( gObj );

            DEBUG_PRINT( "There are now %zd gameObjects", gameObjects.size() );

            // Send an acknowledgment back to this client that they have connected
            if ( sendto( ServerSocket, ackCmd, sizeof( Command ) + sizeof( int32_t ), 0, ( struct sockaddr * )&si_other, slen ) == SOCKET_ERROR )
            {
                DEBUG_PRINT( "ack send failed with error code : %d", WSAGetLastError() );
                exit( EXIT_FAILURE );
            }

        }
        else  //otherwise process the command and reply with broadcast stuff
        {
            Command command = ProcessCmd( buf );
            memset( buf, '\0', DEF_BUF_LEN );

            std::vector<BroadcastedGameObject> vector;
            UpdateGameObjects( command, vector );

            if ( vector.size() > 0 )
            {
                int32_t connectionType = ConnectionType::Broadcast;
                memcpy( buf, &connectionType, sizeof( int32_t ) );

                //set the count
                int32_t count = static_cast<int32_t>( vector.size() );
                memcpy( buf + sizeof( int32_t ), &count, sizeof( int32_t ) );

                //copy the rest of the broadcasted objects over
                if ( vector.size() > 0 )
                    memcpy( buf + 2 * sizeof( int32_t ), &vector[ 0 ], sizeof( BroadcastedGameObject ) * count );

                // Do something with the received message
                if ( sendto( ServerSocket, buf, sizeof( BroadcastedGameObject ) * count + 2 * sizeof( int32_t ), 0, ( struct sockaddr * )&si_other, slen ) == SOCKET_ERROR )
                {
                    DEBUG_PRINT( "sendto failed with error code : %d", WSAGetLastError() );
                    exit( EXIT_FAILURE );
                }
                DEBUG_PRINT( "'Broadcasted' the g objs' positions of size %d", count );
            }
            else
            {
                int32_t connectionType = ConnectionType::ConnectionAck;
                memcpy( buf, &connectionType, sizeof( int32_t ) );

                Command c = { };
                memcpy( buf + sizeof( int32_t ), &c, sizeof( Command ) );

                // Do something with the received message
                if ( sendto( ServerSocket, buf, sizeof( Command ) + sizeof( int32_t ), 0, ( struct sockaddr * )&si_other, slen ) == SOCKET_ERROR )
                {
                    DEBUG_PRINT( "sendto failed with error code : %d", WSAGetLastError() );
                    exit( EXIT_FAILURE );
                }
                DEBUG_PRINT( "Here's an acknowledgement command" );
            }
        }
    }
}

inline const int Networking::TankServer::GetPort() const
{
    return Port;
}

void TankServer::UpdateGameObjects( Command  cmd, std::vector<BroadcastedGameObject> &vector )
{
    //apply the right commands to the right game object in an inefficiant O(n2) way
    for ( size_t j = 0; j < gameObjects.size(); ++j )
    {
        if ( cmd.clientId == gameObjects[ j ].gameObjId )
        {
            //process the command
            if ( cmd.move_down )
            {
                gameObjects[ j ].y -= 0.1f;
                if ( gameObjects[ j ].y < -10 )
                    gameObjects[ j ].y = -10;
            }
            else if ( cmd.move_up )
            {
                gameObjects[ j ].y += 0.1f;
                if ( gameObjects[ j ].y > 10 )
                    gameObjects[ j ].y = 10;
            }

            if ( cmd.move_left )
            {
                gameObjects[ j ].x += 0.1f;
                if ( gameObjects[ j ].x > 10 )
                    gameObjects[ j ].x = 10;
            }
            else if ( cmd.move_right )
            {
                gameObjects[ j ].x -= 0.1f;
                if ( gameObjects[ j ].x < -10 )
                    gameObjects[ j ].x = -10;
            }

            break;
        }
    }

    //emplace a copy of the gameobj state
    if ( gameObjects.size() > 0 )
    {
        vector.reserve( gameObjects.size() );

        for ( size_t i = 0; i < gameObjects.size(); ++i )
            vector.push_back( gameObjects[ i ] );
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

Command TankServer::ProcessCmd( char * aCmd )
{
    assert( aCmd != nullptr );

    Command outCommand;
    memcpy( &outCommand, (void*) aCmd, sizeof( Command ) );

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

    DEBUG_PRINT( "Processing command: L%d, R%d, U%d, D%d", outCommand.move_left, outCommand.move_right, outCommand.move_up, outCommand.move_down );

    return outCommand;
}

void TankServer::BroadCastToAllClients()
{

}

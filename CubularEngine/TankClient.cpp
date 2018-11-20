#include "stdafx.h"
#include "TankClient.h"

using namespace Networking;

TankClient::TankClient( const char* aServerAddress, const int aPort )
{
    DEBUG_PRINT( " --- Client App ---\n" );
    strcpy_s( ServerAddr, 32, aServerAddress );

    // Init winsoc ---------------------
    WSADATA wsaData;
    ClientSocket = INVALID_SOCKET;

    DEBUG_PRINT( "Initializing winsock...\n" );

    if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 )
    {
        fprintf( stderr, "FAILED. Error code : %d ", WSAGetLastError() );
        exit( EXIT_FAILURE );
    }

    DEBUG_PRINT( "Initialized winsock!\n" );

    // Create listen socket -----------------------
    if ( ( serverSock = ClientSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) == SOCKET_ERROR )
    {
        DEBUG_PRINT( "Could not create socket : %d ", WSAGetLastError() );
        exit( EXIT_FAILURE );
    }
    DEBUG_PRINT( "Socket created.\n" );

    Command testCmd;
    testCmd.clientId = clientID;
    testCmd.move_down = 1;
    testCmd.move_up = 1;
    testCmd.move_left = 1;
    testCmd.move_right = 1;

    commandQueue.emplace_back( testCmd );

    isDone = false;
    ClientThread = std::thread( &Networking::TankClient::ClientWorkThread, this );

}

TankClient::~TankClient()
{
    ShutDown();

    // Cleanup Winsock ------------------
    closesocket( ClientSocket );
    WSACleanup();
}

void TankClient::SendCommand( Command cmd )
{
    // Add this command to the command queue
    cmd.clientId = clientID;
    commandQueue.emplace_back( cmd );
}

void TankClient::ClientWorkThread()
{
#pragma region Init Socket
    int slen;
    char buf[ DEF_BUF_LEN ];
    char message[ DEF_BUF_LEN ];
    struct sockaddr_in  si_other;
    slen = sizeof( si_other );

    memset( ( char* ) &si_other, 0, sizeof( si_other ) );
    si_other.sin_family = AF_INET;
    si_other.sin_addr.s_addr = INADDR_ANY;
    si_other.sin_port = htons( CurrentPort );

    wchar_t wtext[ 32 ];
    size_t outSize;
    mbstowcs_s( &outSize, wtext, ServerAddr, strlen( ServerAddr ) + 1 );
    InetPton( AF_INET, wtext, &si_other.sin_addr.S_un.S_addr );
#pragma endregion

    while ( 1 )
    {
        if ( isDone )
            return;

        //send a command if there's something in the queue
        if ( commandQueue.size() > 0 )
        {
            Command cmdToSend = { };
            commandQueue.pop_front( cmdToSend );

            // TODO: Replace with the streams

            char charCmd[ 64 ];
            memcpy( charCmd, ( void* ) ( &cmdToSend ), sizeof( Command ) );

            if ( sendto( serverSock, charCmd, sizeof( Command ), 0,
                ( struct sockaddr* ) &si_other, slen ) == SOCKET_ERROR )
            {
                printf( "sendto() failed : %d ", WSAGetLastError() );
                exit( EXIT_FAILURE );
            }

            //clear buffer
            memset( buf, '\0', DEF_BUF_LEN );

            //block until recieved a packet
            if ( recvfrom( serverSock, buf, DEF_BUF_LEN, 0, ( struct sockaddr* )&si_other, &slen ) == SOCKET_ERROR )
            {
                printf( "recvfrom() failed : %d ", WSAGetLastError() );
                exit( EXIT_FAILURE );
            }

            int32_t type = 0;
            memcpy( &type, buf, sizeof( int32_t ) );

            // Assign clientID
            if ( type == ConnectionType::ConnectionAck )
            {
                //initialize the client ID if this is the first time
                if ( clientID == -1 )
                {
                    Command command = {};
                    memcpy( &command, buf, sizeof( Command ) );
                    clientID = command.clientId;
                    DEBUG_PRINT( "Connection to server acknowledged" );
                }

                //validate that command is being received?
            }
            else if ( type == ConnectionType::Broadcast )
            {
                //read count
                int32_t count = 0;
                memcpy( &count, buf + sizeof( int32_t ), sizeof( int32_t ) );

                //read all other broad casted game objs
                std::vector<BroadcastedGameObject> vector( count );
                memcpy( &vector[ 0 ], buf + 2 * sizeof( int32_t ), count * sizeof( BroadcastedGameObject ) );

                //place in the command queueueueueueueueueu
                broadcastedObject.emplace_back( vector );
                DEBUG_PRINT( "Emplaced vector of broadcasted objects" );
            }
            puts( buf );
        }
    }
}

void TankClient::ShutDown()
{
    isDone = true;

    if ( ClientThread.joinable() )
    {
        ClientThread.join();
    }
}

inline const int Networking::TankClient::GetPort() const
{
    return CurrentPort;
}

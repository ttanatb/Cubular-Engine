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

    ServerWorker();
    UpdateGameObjects();
}

TankServer::~TankServer()
{
    isDone = true;
    // Cleanup -------------------
    closesocket( ServerSocket );
    WSACleanup();
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
            DEBUG_PRINT( "recvfrom failed with error code : %d\n", WSAGetLastError() );
            exit( EXIT_FAILURE );
        }

        // Get the IP of this data and see if the client exists
        memset( ip_str, '\0', INET_ADDRSTRLEN );
        inet_ntop( AF_INET, &( si_other.sin_addr ), ip_str, INET_ADDRSTRLEN );
        std::string newIP = ip_str;

        if ( !ClientExists( newIP ) )
        {
            char ackCmd[ 64 ];
            strcpy_s( ackCmd, 64, "ACK" );
            ConnectedClients.push_back( newIP );
            // Send an acknowledgment back to this client that they have connected
            if ( sendto( ServerSocket, ackCmd, strlen( ackCmd ) + 1, 0, ( struct sockaddr * )&si_other, slen ) == SOCKET_ERROR )
            {
                DEBUG_PRINT( "ack send failed with error code : %d", WSAGetLastError() );
                exit( EXIT_FAILURE );
            }
        }

        ProcessCmd( buf );

        // Do something with the received message
        DEBUG_PRINT( "Data received: %s from %s\n", buf, ip_str );

        if ( sendto( ServerSocket, buf, recv_len, 0, ( struct sockaddr * )&si_other, slen ) == SOCKET_ERROR )
        {
            DEBUG_PRINT( "sendto failed with error code : %d", WSAGetLastError() );
            exit( EXIT_FAILURE );
        }
    }
}

inline const int Networking::TankServer::GetPort() const
{
    return Port;
}

void Networking::TankServer::UpdateGameObjects()
{
    float dt = timer->GetDeltaFloatTime();
    //read all the commands in the queue?

    for ( size_t i = 0; i < rigidBodies.size(); ++i )
    {
        gameObjects[ i ].x += rigidBodies[ i ].speedX * dt;
        gameObjects[ i ].y += rigidBodies[ i ].speedY * dt;
    }

    //broadcast out all the gameObjects?

    timer->UpdateTimer();
}

bool Networking::TankServer::ClientExists( std::string & clientIP )
{
    auto itr = ConnectedClients.begin();
    for ( ; itr != ConnectedClients.end(); ++itr )
    {
        if ( *( itr ) == clientIP )
            return true;
    }

    return false;
}

void Networking::TankServer::ProcessCmd( char * aCmd )
{
    assert( aCmd != nullptr );

    // TODO: validate this command

    Command outCommand;
    memcpy( &outCommand, ( void* ) ( aCmd ), sizeof( Command ) );


    // Replace with the Stream

    printf( "Client ID : %d  CMD: %d\n", outCommand.clientId, outCommand.command );
}

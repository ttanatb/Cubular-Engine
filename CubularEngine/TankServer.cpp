#include "stdafx.h"
#include "TankServer.h"

using namespace Networking;

TankServer::TankServer( const int aPort )
    : Port( aPort )
{
    DEBUG_PRINT( " --- Server App ---" );
    DEBUG_PRINT( "Running on Port: %d", Port );

    ServerWorker();
}

TankServer::~TankServer()
{
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
        DEBUG_PRINT( "Waiting for data..." );
        fflush( stdout );

        memset( buf, '\0', DEF_BUF_LEN );

        if ( ( recv_len = recvfrom( ServerSocket, buf, DEF_BUF_LEN, 0, ( struct sockaddr* ) &si_other, &slen ) ) == SOCKET_ERROR )
        {
            DEBUG_PRINT( "recvfrom failed with error code : %d\n", WSAGetLastError() );
            exit( EXIT_FAILURE );
        }

        memset( ip_str, '\0', INET_ADDRSTRLEN );
        inet_ntop( AF_INET, &( si_other.sin_addr ), ip_str, INET_ADDRSTRLEN );
        std::string newIP = ip_str;

        if ( !ClientExists( newIP ) )
        {
            ConnectedClients.push_back( newIP );
        }
        
        // Do something with the received message
        DEBUG_PRINT( "Data received: %s from %s\n", buf, ip_str );

        if ( sendto( ServerSocket, buf, recv_len, 0, ( struct sockaddr * )&si_other, slen ) == SOCKET_ERROR )
        {
            DEBUG_PRINT( "recvfrom failed with error code : %d", WSAGetLastError() );
            exit( EXIT_FAILURE );
        }
    }
}

inline const int Networking::TankServer::GetPort() const
{
    return Port;
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

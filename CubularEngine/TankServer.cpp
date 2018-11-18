#include "stdafx.h"
#include "TankServer.h"

using namespace Networking;

TankServer::TankServer( const int aPort )
    : Port( aPort )
{
    DEBUG_PRINT( " --- Server App ---\n" );
    DEBUG_PRINT( "Running on Port: %d \n", Port );

    /*WSADATA wsaData;

    struct sockaddr_in server;
    
    DEBUG_PRINT( "Initializing winsock...\n" );

    if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 )
    {
        fprintf( stderr, "FAILED. Error code : %d ", WSAGetLastError() );
        exit( EXIT_FAILURE );
    }

    DEBUG_PRINT( "Initialized winsock!\n" );

    // Create listen socket -----------------------
    if ( ( ServerSocket = socket( AF_INET, SOCK_DGRAM, 0 ) ) == SOCKET_ERROR )
    {
        DEBUG_PRINT( "Could not create socket : %d ", WSAGetLastError() );
    }

    DEBUG_PRINT( "Socket created.\n" );

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( Port );

    if ( bind( ServerSocket, ( struct sockaddr* )&server, sizeof( server ) ) == SOCKET_ERROR )
    {
        DEBUG_PRINT( "Could not create socket : %d ", WSAGetLastError() );
        exit( EXIT_FAILURE );
    }

    DEBUG_PRINT( "Bind done.\n" );*/
    
    
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
    slen = sizeof( si_other );

    printf( "Initializing winsock...\n" );

    if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 )
    {
        fprintf( stderr, "FAILED. Error code : %d ", WSAGetLastError() );
        exit( EXIT_FAILURE );
    }

    printf( "Initialized winsock!\n" );

    // Create listen socket -----------------------
    if ( ( ServerSocket = socket( AF_INET, SOCK_DGRAM, 0 ) ) == SOCKET_ERROR )
    {
        printf( "Could not create socket : %d ", WSAGetLastError() );
    }

    printf( "Socket created.\n" );

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( Port );

    if ( bind( ServerSocket, ( struct sockaddr* )&server, sizeof( server ) ) == SOCKET_ERROR )
    {
        printf( "Could not create socket : %d ", WSAGetLastError() );
        exit( EXIT_FAILURE );
    }

    printf( "Bind done.\n" );

    // Wait for data ------------------------------
    while ( 1 )
    {
        printf( "Waiting for data..." );
        fflush( stdout );

        memset( buf, '\0', DEF_BUF_LEN );

        if ( ( recv_len = recvfrom( ServerSocket, buf, DEF_BUF_LEN, 0, ( struct sockaddr* ) &si_other, &slen ) ) == SOCKET_ERROR )
        {
            printf( "recvfrom failed with error code : %d\n", WSAGetLastError() );
            exit( EXIT_FAILURE );
        }

        // Do something with the received message
        printf( "Data received: %s \n", buf );

        if ( sendto( ServerSocket, buf, recv_len, 0, ( struct sockaddr * )&si_other, slen ) == SOCKET_ERROR )
        {
            printf( "recvfrom failed with error code : %d\n", WSAGetLastError() );
            exit( EXIT_FAILURE );
        }

    }
}

inline const int Networking::TankServer::GetPort() const
{
    return Port;
}

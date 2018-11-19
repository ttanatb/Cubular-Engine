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

void Networking::TankClient::SendCommand( Command cmd )
{
    // Add this command to the command queue
    commandQueue.emplace_back( cmd );

}

void TankClient::ClientWorkThread()
{
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

    while ( 1 )
    {
        if ( isDone ) return;

        //printf( "Enter a message : " );
        memset( message, '\0', DEF_BUF_LEN );
        strcpy_s( message, DEF_BUF_LEN, "TEST" );

        /*Command cmdToSend;
        commandQueue.pop_front( cmdToSend );*/

        // IF There is something in the Cmd buffer...
            // Send it to the server

        if ( sendto( serverSock, message, strlen( message ), 0, ( struct sockaddr* ) &si_other, slen ) == SOCKET_ERROR )
        {
            printf( "sendto() failed : %d ", WSAGetLastError() );
            exit( EXIT_FAILURE );
        }

        memset( buf, '\0', DEF_BUF_LEN );
        if ( recvfrom( serverSock, buf, DEF_BUF_LEN, 0, ( struct sockaddr* )&si_other, &slen ) == SOCKET_ERROR )
        {
            printf( "recvfrom() failed : %d ", WSAGetLastError() );
            exit( EXIT_FAILURE );
        }

        if ( strcmp( buf, "ACK" ) )
        {
            DEBUG_PRINT( "EYYY ACK" );
        }

        puts( buf );
    }
}

void Networking::TankClient::ShutDown()
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

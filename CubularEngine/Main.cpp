#include "stdafx.h"
#include "Engine.h"

#include <memory>

#include "TankServer.h"
#include "TankClient.h"
                         
#define IS_CLIENT_FLAG   "-c"
#define IS_SERVER_FLAG   "-s"
#define PORT_NUM_FLAG    "-p"
#define HELP_FLAG        "-h"
#define SERVER_ADDR_FLAG "-a"



/// <summary>
/// Print the help message for this application
/// </summary>
void PrintHelp()
{
    fprintf( stderr, "\nUSAGE: ./UDP_Tank.exe [-c] [-a] [-s] [-p PORT_NUM] \n" );
    fprintf( stderr, "\nOptions: \n" );
    fprintf( stderr, "\t-h\tPrint this helpful message.\n" );
    fprintf( stderr, "\t-c\tRun this application as a client. (Default: true)\n" );
    fprintf( stderr, "\t-a\tThe server address to connect to. (Default: 127.0.0.1)\n" );
    fprintf( stderr, "\t-s\tRun this application as a server. (Default: true)\n" );
    fprintf( stderr, "\t-p\tPort number to run on.            (Default: 50000)\n" );
}

int main( int argc, char* argv[] )
{
#if defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
#endif

    // Make sure that we have an argument
    if ( argc <= 1 )
    {
        PrintHelp();
        exit( EXIT_FAILURE );
    }

    std::unique_ptr<Networking::TankClient> Client;
    std::unique_ptr<Networking::TankServer> Server;

    bool isServer = false;
    bool isClient = false;

    int PORT = 50000;
    char SERVER_ADDR[16] = "127.0.0.1\0";

    // Loop through the args and configure as necessary
    for ( int i = 0; i < argc; ++i )
    {
        if ( strcmp( argv[ i ], HELP_FLAG ) == 0 )
        {
            PrintHelp();
            exit( EXIT_FAILURE );
        }
        else if ( strcmp( argv[ i ], IS_CLIENT_FLAG ) == 0 )
        {
            isClient = true;
            isServer = false;
        }
        else if ( strcmp( argv[ i ], IS_SERVER_FLAG ) == 0 )
        {
            isServer = true;
            isClient = false;
        }
        else if ( strcmp( argv[ i ], PORT_NUM_FLAG ) == 0 )
        {
            if ( i + 1 < argc )
            {
                sscanf_s( argv[ i + 1 ], "%d", &PORT );
            }
        }
        else if ( strcmp( argv[ i ], SERVER_ADDR_FLAG ) == 0 )
        {
            if ( i + 1 < argc )
            {
                strcpy_s( SERVER_ADDR, argv[ i + 1 ] );
            }
        }
    }

    // Server Scope ---------------
    {
        if ( isServer )
        {
            Server = std::make_unique<Networking::TankServer>( PORT );
        }
    }   // Server scope


    // Client Scope -----------
    {
        if ( isClient )
        {
            Client = std::make_unique<Networking::TankClient>( SERVER_ADDR, PORT );

            Engine engine = Engine(Client.get());
            if ( engine.Init() == 0 )
            {
                engine.Run();
            }

            Client->ShutDown();
        }
    }   // Client scope


    return 0;
}
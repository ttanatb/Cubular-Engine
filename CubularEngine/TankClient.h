#pragma once

#include "SocketUse.h"
#include <thread>
#include <atomic>

namespace Networking
{
    /// <summary>
    /// A tank client in charge of listening to data from the server 
    /// and sending commands from the client
    /// </summary>
    /// <author>Ben Hoffman</author>
    class TankClient
    {

    public:

        /// <summary>
        /// Initialize WinSock and create the client worker thread.
        /// </summary>
        TankClient( const char* serverAddress, const int aPort );

        /// <summary>
        /// Cleanup Winsock and wait for the client thread to be 
        /// finished
        /// </summary>
        ~TankClient();

        /// <summary>
        /// Worker thread for the client to listen to data from the 
        /// server on. Handle the processing of received commands.
        /// </summary>
        void ClientWorkThread();

        /// <summary>
        /// Signal to the client that we should stop listening for 
        /// commands from the server and to shut down
        /// </summary>
        void ShutDown();

        /// <summary>
        /// Get the currently connected port of this client
        /// </summary>
        /// <returns>The current port in use by this client</returns>
        inline const int GetPort() const;

    private:

        /** The socket for the client to use */
        SOCKET ClientSocket = INVALID_SOCKET;

        /** The port to connect to on the server */
        int CurrentPort = 50000;

        /** The server address for this socket to connect to */
        char ServerAddr [32];
        
        /** Thread where client is listening for data from the server */
        std::thread ClientThread;

        /** Atomic flag to check if we are done */
        std::atomic<bool> isDone;

        /** ID for the connection to the server */
        int serverSock;

    };

}


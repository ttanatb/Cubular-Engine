#pragma once

#include "SocketUse.h"

namespace Networking
{
    /// <summary>
    /// Server logic for the tank game
    /// </summary>
    /// <author>Ben Hoffman</author>
    class TankServer
    {

    public:

        /// <summary>
        /// Initialize winsock and bind the server socket to the given
        /// port
        /// </summary>
        /// <param name="aPort">The port to run the server on</param>
        TankServer( const int aPort );

        /// <summary>
        /// Cleanup winsock and 
        /// </summary>
        ~TankServer();

        /// <summary>
        /// Method where the server can process commands 
        /// </summary>
        void ServerWorker();

        /// <summary>
        /// Get the current port that the server is running on
        /// </summary>
        /// <returns>Current port that the server is running</returns>
        inline const int GetPort() const;

    private:

        /** The socket for the server to use */
        SOCKET ServerSocket = INVALID_SOCKET;

        /** The current Connected port for the server */
        int Port = 50000;

    };  // Class TankServer

}   // End namespace Networking


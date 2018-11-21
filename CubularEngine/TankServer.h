#pragma once

#include "SocketUse.h"
#include <string.h>
#include <atomic>

class GameTime;

namespace Networking
{
    struct BroadcastedGameObjectRigidBody
    {
        int32_t gameObjId;
        float speedX;
        float speedY;
    };

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
        /// Signal to the client that we should stop listening for  
        /// commands from the server and to shut down
        /// </summary>
        void ShutDown();

        /// <summary>
        /// Method where the server can process commands 
        /// </summary>
        void ServerWorker();

        /// <summary>
        /// Get the current port that the server is running on
        /// </summary>
        /// <returns>Current port that the server is running</returns>
        inline const int GetPort() const;

        void UpdateGameObjects( Command  cmd, std::vector< BroadcastedGameObject > &vector );
    private:

        /** The socket for the server to use */
        SOCKET ServerSocket = INVALID_SOCKET;

        /** The current Connected port for the server */
        int Port = 50000;

        /** A vector of connected clients where the string is their IP addr */
        std::vector<std::string> ConnectedClients;

        std::thread ServerThread;

        /** Atomic flag to check if we are done */
        std::atomic<bool> isDone;

        /// <summary>
        /// Check if a given client already exists in the server
        /// </summary>
        /// <param name="clientIP">The IP address in question</param>
        /// <returns>True if the client exists</returns>
        bool ClientExists( std::string& clientIP );

        /// <summary>
        /// Process a command received from the client
        /// </summary>
        /// <param name="aCmd">Char command that was recieved from the client</param>
        Command ProcessCmd( char* aCmd );

        /// <summary>
        /// Send this command update to all current clients
        /// </summary>
        /// <param name="cmd">The command to send</param>
        void BroadCastToAllClients( std::vector< BroadcastedGameObject > &vecOfObjs );

        std::vector<BroadcastedGameObject> gameObjects;
        std::vector<BroadcastedGameObjectRigidBody> rigidBodies;

        ConcurrentQueue<Command> commandQueue;
        ConcurrentQueue<std::vector<BroadcastedGameObject>> broadcastedObject;

        const float MAX_SPEED = 3.0f;

        GameTime *timer;

    };  // Class TankServer

}   // End namespace Networking


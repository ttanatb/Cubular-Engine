#pragma once

#include <cstdint>

namespace Networking
{
    static const size_t MAX_CONNECTIONS = 16;

    /// <summary>
    /// A game object whose state has changed, and needs to 
    /// sent from the server to other clients
    /// </summary>
    struct BroadcastedGameObject
    {
        int32_t gameObjId;
        float x;
        float y;
    };

    /// <summary>
    /// A command that can be sent from the client to the server
    /// </summary>
    struct Command
    {
        int32_t clientId;
        int32_t command;
    };    
}
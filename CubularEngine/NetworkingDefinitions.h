#pragma once

#include <cstdint>

namespace Networking
{
#define MAX_CONNECTIONS 16

    struct BroadcastedGameObject
    {
        int32_t gameObj;
        float x;
        float y;
    };

    struct Command
    {
        int32_t clientId;
        int32_t command;
    };    
}
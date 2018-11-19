#pragma once

#include <cstdint>
#include <glm/glm.hpp>

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

    /*
    glm::vec3 g_tankColors[ MAX_CONNECTIONS ] = {
        glm::vec3( 1.0f, 0.0f, 0.0f ),
        glm::vec3( 0.0f, 1.0f, 0.0f ),
        glm::vec3( 0.0f, 0.0f, 1.0f ),
        glm::vec3( 1.0f, 1.0f, 0.0f ),
        glm::vec3( 1.0f, 0.0f, 1.0f ),
        glm::vec3( 0.0f, 1.0f, 1.0f ),
        glm::vec3( 1.0f, 1.0f, 1.0f ),
        glm::vec3( 0.0f, 0.0f, 0.0f ),
        glm::vec3( 1.0f, 0.5f, 0.5f ),
        glm::vec3( 0.5f, 1.0f, 0.5f ),
        glm::vec3( 0.5f, 0.5f, 1.0f ),
        glm::vec3( 1.0f, 1.0f, 0.5f ),
        glm::vec3( 1.0f, 0.5f, 1.0f ),
        glm::vec3( 0.5f, 1.0f, 1.0f ),
        glm::vec3( 0.2f, 0.2f, 0.2f ),
        glm::vec3( 0.7f, 0.7f, 0.7f )
    };
    */
}
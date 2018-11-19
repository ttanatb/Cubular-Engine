#pragma once

#include <unordered_map>
#include "GameEntity.h"

class Material;
class Mesh;

class GameEntityManager
{
private:
    std::unordered_map<uint32_t, GameEntity *> gameEntities;
    std::unordered_map<uint32_t, bool> isAlive;

    Mesh* tankMesh;
    Material *tankBaseMaterial;

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

    static GameEntityManager* instance;     //singleton stuff
    GameEntityManager();
    ~GameEntityManager();
public:
    /// <summary>
    /// Singleton reference to the instance
    /// </summary>
    static GameEntityManager* GetInstance();

    /// <summary>
    /// De-allocation
    /// </summary>
    static void Release();

    /// <summary>
    /// Updates the game objects based off of the received game objects
    /// </summary>
    /// <param name="broadcastedGameObjects">an array of gameobjects</param>
    void Update( std::vector<Networking::BroadcastedGameObject> broadcastedGameObjects );
};


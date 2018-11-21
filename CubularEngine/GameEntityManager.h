#pragma once

#include "GameEntity.h"

class Material;
class Mesh;

/// <summary>
/// Manages the game entities in the client
/// </summary>
class GameEntityManager
{
private:

    //used to keep track of the game entities
    std::unordered_map<uint32_t, GameEntity *> *gameEntities;
    std::unordered_map<uint32_t, bool> isAlive;

    //mesh and mat to assign to entity
    Mesh* tankMesh;
    Material *tankBaseMaterial;

    //basically the color of the tanks based off of the client ID
    glm::vec3 g_tankColors[ Networking::MAX_CONNECTIONS ] = {
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
    /// Set the global base material to use for th tanks
    /// </summary>
    /// <param name="mat"></param>
    void SetMaterial( Material *mat ) { tankBaseMaterial = mat; }
    
    /// <summary>
    /// Set the global mesh to apply to the tanks
    /// </summary>
    /// <param name="mesh"></param>
    void SetMesh( Mesh *mesh ) { tankMesh = mesh; }

    /// <summary>
    /// It's all the game entities!
    /// </summary>
    /// <returns></returns>
    std::unordered_map<uint32_t, GameEntity *>* GetGameEntities() { return gameEntities; }

    /// <summary>
    /// Initializess the manager
    /// </summary>
    /// <param name="mat"></param>
    /// <param name="mesh"></param>
    void Init( Material *mat, Mesh* mesh );

    /// <summary>
    /// Updates the game objects based off of the received game objects
    /// </summary>
    /// <param name="broadcastedGameObjects">an array of gameobjects</param>
    void Update( std::vector<Networking::BroadcastedGameObject> broadcastedGameObjects );
};


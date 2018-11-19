#pragma once

#include <unordered_map>
#include "GameEntity.h"

class Material;
class Mesh;

class GameEntityManager
{
private:
    std::unordered_map<uint32_t, GameEntity *> gameEntities;

    Mesh* tankMesh;
    Material *tankBaseMaterial;

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


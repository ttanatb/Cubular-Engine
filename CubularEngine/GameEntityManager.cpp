#include "stdafx.h"
#include "GameEntityManager.h"
#include "Mesh.h"
#include "Material.h"

GameEntityManager* GameEntityManager::instance = nullptr;


GameEntityManager::GameEntityManager()
{
}


GameEntityManager::~GameEntityManager()
{
}

GameEntityManager * GameEntityManager::GetInstance()
{
    if ( instance == nullptr )
        instance = new GameEntityManager();
    return instance;
}

void GameEntityManager::Release()
{
    if ( instance != nullptr )
    {
        delete instance;
        instance = nullptr;
    }
}

void GameEntityManager::Update( std::vector<Networking::BroadcastedGameObject> broadcastedGameObjects )
{
    for ( size_t i = 0; i < broadcastedGameObjects.size(); ++i )
    {
        Networking::BroadcastedGameObject obj = broadcastedGameObjects[ i ];
        if ( auto searchResult = gameEntities.find( obj.gameObj ) == gameEntities.end() )
        {
            Material *material = new Material( *tankBaseMaterial );
            //material->SetAlbedoColor( Networking::g_tankColors[ static_cast<size_t>(obj.gameObj) % MAX_CONNECTIONS ] );
            GameEntity *entity = new GameEntity( 
                tankMesh, 
                new Material( *tankBaseMaterial ), 
                glm::vec3( obj.x, obj.y, 0.f ), 
                glm::vec3( 0.f ), 
                glm::vec3( 1.f ) 
            );
            gameEntities.insert( { obj.gameObj, entity } );
        }
        else
        {
            GameEntity *entity = gameEntities[ obj.gameObj ];
            entity->SetPosition( glm::vec3( obj.x, obj.y, 0.f ) );
        }
    }
}

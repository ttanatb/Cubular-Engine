#include "stdafx.h"
#include "GameEntityManager.h"
#include "Mesh.h"
#include "Material.h"

GameEntityManager* GameEntityManager::instance = nullptr;


GameEntityManager::GameEntityManager()
{
    gameEntities = new std::unordered_map<uint32_t, GameEntity *>;
}

GameEntityManager::~GameEntityManager()
{
    for ( const auto& pair : *gameEntities )
    {
        delete (*gameEntities)[ pair.first ];
    }

    delete gameEntities;
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

void GameEntityManager::Init( Material * mat, Mesh * mesh )
{
    tankMesh = mesh;
    tankBaseMaterial = mat;
}

void GameEntityManager::Update( std::vector<Networking::BroadcastedGameObject> broadcastedGameObjects )
{
    for ( size_t i = 0; i < broadcastedGameObjects.size(); ++i )
    {
        Networking::BroadcastedGameObject obj = broadcastedGameObjects[ i ];

        //update
        if ( auto searchResult = gameEntities->find( obj.gameObjId ) != gameEntities->end() )
        {
            GameEntity *entity = (*gameEntities)[ obj.gameObjId ];
            entity->SetPosition( glm::vec3( obj.x, obj.y, 0.f ) );
        }

        //add new game entity
        else
        {
            Material *material = new Material( *tankBaseMaterial );
            material->SetAlbedoColor( g_tankColors[ static_cast<size_t>( obj.gameObjId ) % Networking::MAX_CONNECTIONS ] );
            GameEntity *entity = new GameEntity(
                tankMesh,
                new Material( *tankBaseMaterial ),
                glm::vec3( obj.x, obj.y, 0.f ),
                glm::vec3( 0.f ),
                glm::vec3( 1.f )
            );
            gameEntities->insert( { obj.gameObjId, entity } );
        }

        isAlive[ obj.gameObjId ] = true;
    }

    //find and remove things
    if ( gameEntities->size() > broadcastedGameObjects.size() )
    {
        for ( const auto& pair : isAlive )
        {
            //remove if no longer alive
            if ( !pair.second )
            {
                delete (*gameEntities)[ pair.first ];
                gameEntities->erase( pair.first );
            }
        }
    }
}

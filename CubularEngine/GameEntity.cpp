#include "stdafx.h"
#include "GameEntity.h"
#include "Material.h"
#include "Mesh.h"
#include "Camera.h"
#include "ResourceManager.h"

GameEntity::GameEntity(
    const char* scriptName )
    :IEntity( scriptName )
{
    //default values
    resourceManager = ResourceManager::GetInstance();
    position = glm::vec3( 0.f );
    eulerAngles = glm::vec3( 0.f );
    scale = glm::vec3( 1.f );
    worldMatrix = glm::identity<glm::mat4>();

    script.new_usertype<GameEntity>( "test_entity",
        "getParent", &GameEntity::GetParent,
        "getChildren", &GameEntity::GetChildren,
        "getName", &GameEntity::GetName,
        "setName", &GameEntity::SetName,
        "setMaterial", &GameEntity::SetMaterial,
        "setMesh", &GameEntity::SetMesh
    );

    script[ "entity" ] = this;

    startFunc();
}

GameEntity::~GameEntity()
{
}

void GameEntity::Update()
{
    IEntity::Update();
}

void GameEntity::Render( Camera * camera )
{
    if ( material )
        material->Bind( camera, worldMatrix );
    if ( mesh )
        mesh->Render();
    IEntity::Render( camera );
}

void GameEntity::SetMaterial( std::string matName )
{
    material = resourceManager->GetMaterial( matName );
}

void GameEntity::SetMesh( std::string meshName )
{
    mesh = resourceManager->GetMesh( meshName );
}

#include "stdafx.h"
#include "GameEntity.h"
#include "Material.h"
#include "Mesh.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "SceneGraph.h"

GameEntity::GameEntity(
    const char* scriptName,
    bool isFromLua )
    :IEntity( scriptName, isFromLua )
{
    //default values
    resourceManager = ResourceManager::GetInstance();
    position = glm::vec3( 0.f );
    eulerAngles = glm::vec3( 0.f );
    scale = glm::vec3( 1.f );
    worldMatrix = glm::identity<glm::mat4>();

    //lua binding
    script.new_usertype<GameEntity>( scriptName,
        sol::constructors<GameEntity(const char * scriptName, bool)>(),
        "getParent", &GameEntity::GetParent,
        "destroy", &GameEntity::Destroy,
        "getName", &GameEntity::GetName,
        "setName", &GameEntity::SetName,
        "setMaterial", &GameEntity::SetMaterial,
        "setMesh", &GameEntity::SetMesh,
        "setPosition", &GameEntity::SetPosition,
        "getPositionX", &GameEntity::GetPositionX,
        "getPositionY", &GameEntity::GetPositionY,
        "getPositionZ", &GameEntity::GetPositionZ,
        "setRotation", &GameEntity::SetRotation,
        "getRotationX", &GameEntity::GetRotationX,
        "getRotationY", &GameEntity::GetRotationY,
        "getRotationZ", &GameEntity::GetRotationZ,
        "setScale", &GameEntity::SetScale,
        "getScaleX", &GameEntity::GetScaleX,
        "getScaleY", &GameEntity::GetScaleY,
        "getScaleZ", &GameEntity::GetScaleZ,
        "setAlbedo", &GameEntity::SetAlbedo,
        "getAlbedoX", &GameEntity::GetAlbedoX,
        "getAlbedoY", &GameEntity::GetAlbedoY,
        "getAlbedoZ", &GameEntity::GetAlbedoZ
    );

    script[ "entity" ] = this;
    startFunc();

    SceneGraph::GetInstance()->AddRootEntity( this );
}

GameEntity::~GameEntity()
{
    delete material;
}

void GameEntity::Update()
{
    IEntity::Update();
    worldMatrix = glm::identity<glm::mat4x4>();
    worldMatrix = glm::translate( worldMatrix, position );
    worldMatrix = worldMatrix * glm::yawPitchRoll( eulerAngles.y, eulerAngles.x, eulerAngles.z );
    worldMatrix = glm::scale( worldMatrix, scale );
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
    material = new Material(*resourceManager->GetMaterial( matName ));
}

void GameEntity::SetMesh( std::string meshName )
{
    mesh = resourceManager->GetMesh( meshName );
}

void GameEntity::SetPosition( float x, float y, float z )
{
    position = glm::vec3( x, y, z );
}

void GameEntity::SetRotation( float x, float y, float z )
{
    eulerAngles = glm::vec3( x, y, z );
}

void GameEntity::SetScale( float x, float y, float z )
{
    scale = glm::vec3( x, y, z );
}

void GameEntity::SetAlbedo( float x, float y, float z )
{
    material->SetAlbedo( x, y, z );
}

float GameEntity::GetAlbedoX() { return material->GetAlbedoX(); }
float GameEntity::GetAlbedoY() { return material->GetAlbedoY(); }
float GameEntity::GetAlbedoZ() { return material->GetAlbedoZ(); }

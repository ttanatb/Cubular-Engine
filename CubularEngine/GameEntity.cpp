#include "stdafx.h"
#include "GameEntity.h"
#include "Material.h"
#include "Mesh.h"
#include "Camera.h"

GameEntity::GameEntity(
    Mesh* mesh,
    Material * material,
    const char* startScriptName,
    const char* updateScriptName )
    :IEntity( startScriptName, updateScriptName )
{
    this->mesh = mesh;
    this->material = material;
    this->position = glm::vec3( 0.f );
    this->eulerAngles = glm::vec3( 0.f );
    this->scale = glm::vec3( 1.f );
    worldMatrix = glm::identity<glm::mat4>();
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
    material->Bind( camera, worldMatrix );
    mesh->Render();
    IEntity::Render( camera );
}

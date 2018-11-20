#include "stdafx.h"
#include "Camera.h"
#include "GameEntity.h"
#include "Material.h"
#include "Mesh.h"

GameEntity::GameEntity(Mesh * mesh, 
    Material * material,
    glm::vec3 position, 
    glm::vec3 eulerAngles, 
    glm::vec3 scale)
{
    this->mesh = mesh;
    this->material = material;
    this->position = position;
    this->eulerAngles = eulerAngles;
    this->scale = scale;
    worldMatrix = glm::identity<glm::mat4>();
}

GameEntity::~GameEntity()
{
    if ( material != nullptr ) delete material;
}

void GameEntity::Update()
{
    worldMatrix = glm::translate( glm::identity<glm::mat4>(), position );
}

void GameEntity::Render(Camera* camera)
{
    material->Bind(camera, worldMatrix);
    mesh->Render();
}

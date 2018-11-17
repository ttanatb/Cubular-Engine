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
}

void GameEntity::Update()
{
    eulerAngles.y += 0.001f;   //very small increment because we don't have deltaTime

    worldMatrix = glm::rotate(glm::identity<glm::mat4>(),
        this->eulerAngles.y,
        glm::vec3(0.f, 1.f, 0.f)
    );
}

void GameEntity::Render(Camera* camera)
{
    material->Bind(camera, worldMatrix);
    mesh->Render();
}

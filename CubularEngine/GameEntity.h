#pragma once
#include "IEntity.h"

class Mesh;
class Material;

class GameEntity : public IEntity
{
private:
    //TODO - maybe this should be in a transform class?
    glm::vec3 position;
    glm::vec3 eulerAngles;
    glm::vec3 scale;
    glm::mat4 worldMatrix;

    Mesh* mesh = nullptr;
    Material* material = nullptr;

public:
    /// <summary>
/// Basic paramterized constructor for most of our private vars
/// </summary>
    GameEntity(
        Mesh* mesh,
        Material* material,
        const char* startScriptName,
        const char* updateScriptName
    );

    ~GameEntity();

    void Update() override;

    void Render( Camera* camera ) override;
};


#pragma once
#include "IEntity.h"

class Mesh;
class Material;
class ResourceManager;

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
    ResourceManager* resourceManager = nullptr;

public:
    /// <summary>
    /// Basic paramterized constructor for most of our private vars
    /// </summary>
    GameEntity( const char* scriptName );

    ~GameEntity();

    void Update() override;
    void Render( Camera* camera ) override;

    void SetMaterial( std::string matName );
    void SetMesh( std::string meshName );
};


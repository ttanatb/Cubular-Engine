#pragma once
#include "IEntity.h"

class Mesh;
class Material;
class ResourceManager;

/// <summary>
/// A 3D game object
/// </summary>
class GameEntity : public IEntity
{
private:
    //transform variables
    glm::vec3 position;
    glm::vec3 eulerAngles;
    glm::vec3 scale;
    glm::mat4 worldMatrix;

    //rendering
    Mesh* mesh = nullptr;
    Material* material = nullptr;

    //singleton reference
    ResourceManager* resourceManager = nullptr;
public:
    /// <summary>
    /// Basic paramterized constructor for most of our private vars
    /// </summary>
    GameEntity( const char* scriptName, bool isFromLua );

    /// <summary>
    /// Destructors
    /// </summary>
    ~GameEntity();

    /// <summary>
    /// Update tick every frame
    /// </summary>
    void Update() override;
    
    /// <summary>
    /// Binding material for rendering
    /// </summary>
    /// <param name="camera"></param>
    void Render( Camera* camera ) override;

    /// --- Setters and getters ---

    void SetMaterial( std::string matName );
    void SetMesh( std::string meshName );
    
    void SetPosition( float x, float y, float z );
    void SetRotation( float x, float y, float z );
    void SetScale( float x, float y, float z );

    float GetPositionX() { return position.x; }
    float GetPositionY() { return position.y; } 
    float GetPositionZ() { return position.z; }

    float GetRotationX() { return eulerAngles.x; }
    float GetRotationY() { return eulerAngles.y; }
    float GetRotationZ() { return eulerAngles.z; }

    float GetScaleX() { return scale.x; }
    float GetScaleY() { return scale.y; }
    float GetScaleZ() { return scale.z; }

    void SetAlbedo( float x, float y, float z ); 

    float GetAlbedoX(); 
    float GetAlbedoY(); 
    float GetAlbedoZ(); 
};


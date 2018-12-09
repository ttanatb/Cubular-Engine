#pragma once

class Mesh;
class Material;
class Camera;

/// <summary>
/// Represents one 'renderable' objet
/// </summary>
class IEntity
{
protected:
    const static size_t NAME_LENGTH = 16;

    std::vector<IEntity*> children;
    IEntity* parent = nullptr;
    char name[ NAME_LENGTH ];

    sol::state startScript;
    sol::state updateScript;
public: 
    IEntity(const char* startScriptName, const char* updateScriptName);

    /// <summary>
    /// Destruction
    /// </summary>
    virtual ~IEntity();

    /// <summary>
    /// Updates the worldMatrix
    /// </summary>
    virtual void Update();

    /// <summary>
    /// Renders the gameEntity based on a camera
    /// </summary>
    virtual void Render(Camera* camera);

    /// <summary>
    /// Get the parent of this entity
    /// </summary>
    IEntity* GetParent() const { return parent; } 

    /// <summary>
    /// Get the children of the entity
    /// </summary>
    std::vector<IEntity*> GetChildren() const { return children; }

    /// <summary>
    /// Get the name of the entity
    /// </summary>
    const char* GetName() const { return name; }
    
    /// <summary>
    /// Adds a child to this entity's children vector
    /// </summary>
    /// <param name="newChild">New child to add</param>
    void AddChild( IEntity* newChild );

    /// <summary>
    /// Attaches this entity to a new parent
    /// </summary>
    /// <param name="newParent">Parent to attach to</param>
    void AttachToParent( IEntity* newParent );

    /// <summary>
    /// Set the name of the entity
    /// </summary>
    void SetName(const char* newName);
};


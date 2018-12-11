#pragma once
class IEntity;
class Camera;

class SceneGraph
{
public:
    static SceneGraph* GetInstance();
    static void ReleaseInstance();

    void InitFromConfig();
    void Update();
    void Render(Camera* camera);

    size_t GetCount() const { return count; }

    void AddRootEntity( IEntity *entity );
    void RemoveEntity( IEntity *entity );
private:
    SceneGraph();
    ~SceneGraph();
    static SceneGraph* instance;

    std::vector<IEntity *> rootEntities;
    std::vector<IEntity *> toDealloc;
    size_t count;
};

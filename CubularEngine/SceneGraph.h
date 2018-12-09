#pragma once
class IEntity;
class Camera;

class SceneGraph
{
public:
    SceneGraph();
    ~SceneGraph();

    void InitFromConfig();
    void Update();
    void Render(Camera* camera);

    size_t GetCount() const { return count; }
private:
    IEntity* root;
    size_t count;
};


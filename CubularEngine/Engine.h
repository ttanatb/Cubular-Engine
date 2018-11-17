#pragma once

class Mesh;
class Material;
class Camera;
class GameEntity;
class Input;

class Engine
{
public:
    Engine();
    ~Engine();

    int Init();
    void Run();
private:
    int LoadAssets();
    int InitGameEntities();
    GLFWwindow *window;

    GLuint shaderProgram;
    GLuint vs;
    GLuint fs;

    Mesh* tankMesh;
    Material* tankMaterial;

    Camera *camera;
    std::vector<GameEntity *> gameEntities;

    Input* input;
};

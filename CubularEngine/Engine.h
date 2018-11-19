#pragma once

class Mesh;
class Material;
class Camera;
class Input;
class GameEntityManager;

class Engine
{
public:
    Engine();
    ~Engine();

    int Init();
    void Run();
private:
    int LoadAssets();
    GLFWwindow *window;

    GLuint shaderProgram;
    GLuint vs;
    GLuint fs;

    Mesh* tankMesh;
    Material* tankMaterial;

    Camera* camera;

    Input* input;
    GameEntityManager* gameEntityManager;
};

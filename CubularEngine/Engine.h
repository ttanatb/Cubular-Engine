#pragma once

class Mesh;
class Material;
class Camera;
class Input;
class GameEntityManager;

#include "TankClient.h"

class Engine
{
public:
    Engine( Networking::TankClient* client );
    ~Engine();

    int Init();
    void Run();
private:
    int LoadAssets();

    GLFWwindow *window = nullptr;

    GLuint shaderProgram = 0;
    GLuint vs = 0;
    GLuint fs = 0;

    Mesh* tankMesh = nullptr;
    Material* tankMaterial = nullptr;

    Camera* camera = nullptr;

    Input* input = nullptr;
    GameEntityManager* gameEntityManager = nullptr;
    Networking::TankClient* client = nullptr;
};

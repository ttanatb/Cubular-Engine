#pragma once

//forward declare all these classes (included in .cpp)
class Mesh;
class Material;
class Camera;
class Input;
class SceneGraph;

class Engine
{
public:
    Engine();
    ~Engine();

    int Init();
    void Run();

    static void ErrorCallBack( int error, const char* description );
private:
    int LoadAssets();
    GLFWwindow *window;

    GLuint shaderProgram;
    GLuint vs;
    GLuint fs;

    Camera *camera;

    Input* input;
    Configs* config;
    SceneGraph* sceneGraph;
};

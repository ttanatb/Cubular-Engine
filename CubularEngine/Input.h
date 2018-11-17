#pragma once

/// <summary>
/// Singleton that does basic input handling
/// </summary>
class Input
{
private:      
    /// <summary>
    /// Singleton implementation (private constructor & destructor)
    /// </summary>
    Input();
    ~Input();

    static Input* instance;     //singleton stuff
    GLFWwindow* window;         //window (required for input)
public:
    /// <summary>
    /// Singleton reference to the instance
    /// </summary>
    static Input* GetInstance();

    /// <summary>
    /// De-allocation
    /// </summary>
    static void Release();

    /// <summary>
    /// Initializes the input singleton before input can be detected properly
    /// </summary>
    void Init(GLFWwindow* window);

    /// <summary>
    /// Checks if the key is down
    /// </summary>
    bool IsKeyDown(int key);
};

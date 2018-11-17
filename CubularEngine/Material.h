#pragma once

class Camera;

/// <summary>
/// Represents one material to apply to a renderable item, and handles sending 
/// any uniform over from CPU to GPU.
/// </summary>
class Material
{
private:
    //TODO: if you have additional textures to bind, it will go here

    //handle to the shader program
    GLuint shaderProgram;
public:

    /// <summary>
    /// Creates a 'material' for a certain shaderProgram
    /// </summary>
    /// <param name="shaderProgram"></param>
    Material(GLuint shaderProgram);

    /// <summary>
    /// Destruction
    /// </summary>
    ~Material();

    /// <summary>
    /// Binds the data that's needed to the uniforms
    /// </summary>
    /// <param name="camera">Pointer to the rendering camera</param>
    /// <param name="worldMatrix">Matrix from model to world space</param>
    void Bind(
        Camera* camera, 
        glm::mat4 worldMatrix
    );
};

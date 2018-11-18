#pragma once

class Camera;

/// <summary>
/// Represents one material to apply to a renderable item, and handles sending 
/// any uniform over from CPU to GPU.
/// </summary>
class Material
{
private:
    //handle to the shader program
    GLuint shaderProgram;

    //color of the material
    glm::vec3 albedoColor;
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
    /// Set the albedo color of the box
    /// </summary>
    /// <param name="color">Color to set it to</param>
    void SetAlbedoColor( glm::vec3 color ) { albedoColor = color; } 

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

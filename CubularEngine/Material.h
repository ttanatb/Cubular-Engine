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

    glm::vec3 albedo = glm::vec3( 0.f, 0.f, 0.f );
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

    void SetAlbedo( float x, float y, float z ) { albedo = glm::vec3( x, y, z ); }
    float GetAlbedoX() { return albedo.x; }
    float GetAlbedoY() { return albedo.y; }
    float GetAlbedoZ() { return albedo.z; }
};

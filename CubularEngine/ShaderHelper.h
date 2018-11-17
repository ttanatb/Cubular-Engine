#pragma once

#include <string>

///<summary>This loads in a shader based off of the file</summary>
class ShaderHelper
{
public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="filePath"></param>
    /// <param name="shaderType"></param>
    /// <returns></returns>
    static GLuint LoadFromFile(std::string filePath, GLenum shaderType);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="shaderCode"></param>
	/// <param name="shaderType"></param>
	/// <returns></returns>
    static GLuint LoadFromString(std::string shaderCode, GLenum shaderType);

    /// <summary>
    /// Links a shader program from a vs and fs
    /// </summary>
    /// <param name="vertexShader">Vertex shader</param>
    /// <param name="fragmentShader">Fragment Shader</param>
    /// <returns></returns>
    static GLuint LinkShaderProgram( GLuint vertexShader, GLuint fragmentShader );
};


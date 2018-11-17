#include "stdafx.h"
#include "ShaderHelper.h"
#include <fstream>

GLuint ShaderHelper::LoadFromFile( std::string filePath, GLenum shaderType )
{
    std::ifstream file( filePath );

    // Check if the file exists
    if ( !file.good() )
    {
        DEBUG_PRINT( "Can't read file: %s\n", filePath );
        return false;
    }

    // ifstream internally keeps track of where in the file.

    // Here we find the end of the file.
    file.seekg( 0, std::ios::end );

    // Make a string and set its size equal to the length of the file.
    std::string shaderCode;
    shaderCode.resize( (size_t) file.tellg() );

    // Go back to the beginning of the file.
    file.seekg( 0, std::ios::beg );

    // Read the file into the string until we reach the end of the string.
    file.read( &shaderCode[ 0 ], shaderCode.size() );

    // Close the file.
    file.close();

    // Init using the string.
    return LoadFromString( shaderCode, shaderType );
}

GLuint ShaderHelper::LoadFromString( std::string shaderCode, GLenum shaderType )
{
    // Get the char* and length
    const char* shaderCodePointer = shaderCode.data();
    int shaderCodeLength = shaderCode.size();

    GLuint shaderLoc = glCreateShader( shaderType );

    // Set the source code and compile.
    glShaderSource( shaderLoc, 1, &shaderCodePointer, &shaderCodeLength );
    glCompileShader( shaderLoc );

    GLint isCompiled;

    // Check if the fragmentShader compiles:
    // If it failed, print an error and delete it.
    glGetShaderiv( shaderLoc, GL_COMPILE_STATUS, &isCompiled );

    if ( !isCompiled )
    {
        char infolog[ 1024 ];
        glGetShaderInfoLog( shaderLoc, 1024, NULL, infolog );
        DEBUG_PRINT( "Shader compilation failed with error: %s \n", infolog );

        // Delete the shader, and set the index to zero so that this object knows it doesn't have a shader.
        glDeleteShader( shaderLoc );
        shaderLoc = 0;
    }

    return shaderLoc;
}

GLuint ShaderHelper::LinkShaderProgram( GLuint vertexShader, GLuint fragmentShader )
{
    GLuint shaderProgram = glCreateProgram();
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );
    glLinkProgram( shaderProgram );

    GLint isLinked;
    glGetProgramiv( shaderProgram, GL_LINK_STATUS, &isLinked );
    if ( !isLinked )
    {
        char infolog[ 1024 ];
        glGetProgramInfoLog( shaderProgram, 1024, NULL, infolog );
        DEBUG_PRINT( "Shader program linking failed with error: %s\n", infolog );

        // Delete the program, and set the index to zero
        glDeleteProgram( shaderProgram );
        shaderProgram = 0;
    }

    return shaderProgram;
}

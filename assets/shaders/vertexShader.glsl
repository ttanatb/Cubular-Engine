/*
This is the vertex shader
*/

//specifies the version of the shader (and what features are enabled)
#version 400 core

// vertex attribute for position (loc = 0)
in vec3 position;

out vec4 vsps_worldPos;

//These are our uniform variables! They are like public static variables but
//they are nothing like public static variables (lol). The similarity lie in
//the fact that you can access these variables from the CPU and that one instance
//of it exists across one execution of the shader (unlike in variables, where one
//'in' variable represents one vertex (in the vertex shader) or one pixel (in the
//fragment shader).
uniform mat4 modelToWorld;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

//entry point for the vertex shader
void main(void)
{
    vec4 worldPos = vec4(position, 1.0);

    //move it to the world coordinates
    worldPos = modelToWorld * worldPos;
    vsps_worldPos = worldPos;

    //apply our camera matrcies to bring it to screen space
    worldPos = viewMatrix * worldPos;
    worldPos = projectionMatrix * worldPos;

    gl_Position = worldPos;
}
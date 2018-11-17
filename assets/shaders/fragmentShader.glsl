/*
This is the fragment shader, and it's main job is to determine the color for each pixel (fragment)
*/

//specifies the version of the shader (and what features are enabled)
#version 400 core

in vec4 vsps_worldPos;

out vec4 color;



//entry point for the fragment shader
void main(void)
{
    color = vec4(normalize(vsps_worldPos.rgb), 1);
}
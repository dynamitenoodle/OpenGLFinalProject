/*
This is the fragment shader, and it's main job is to determine the color for each pixel (fragment)
*/

//specifies the version of the shader (and what features are enabled)
#version 400 core

out vec4 color;
in vec4 vsfs_worldPos;

uniform vec4 ourColor; // we set this variable in the OpenGL code

//entry point for the fragment shader
void main(void)
{
	//color = vec4(abs(vsfs_worldPos.rgb), 1);
	color = ourColor;
}
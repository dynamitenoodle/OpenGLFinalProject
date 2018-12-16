/*
This is the vertex shader
*/

//specifies the version of the shader (and what features are enabled)
#version 400 core

// vertex attribute for position (loc = 0)
in vec3 position;

out vec4 vsfs_worldPos;

uniform mat4 modelToWorld;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

//entry point for the vertex shader
void main(void)
{
	vec4 worldPos = vec4(position, 1.0);

	// move it to the world coordinates
	worldPos = modelToWorld * worldPos;
	vsfs_worldPos = worldPos;		// location in the world

	// apply our camera matrcies to bring it to screen space
	worldPos = viewMatrix * worldPos;
	worldPos = projectionMatrix * worldPos;

	gl_Position = worldPos;		// location
}
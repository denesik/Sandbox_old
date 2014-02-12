#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texcoord;

out vec2 fragTexcoord;

uniform mat4 MVP;

void main()
{	

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(position, 1.0);

	fragTexcoord = texcoord;
}


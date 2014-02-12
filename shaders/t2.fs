#version 330 core

// Interpolated values from the vertex shaders
uniform sampler2D colorTexture;

in vec2 fragTexcoord;

// Ouput data
out vec4 color;

void main()
{

	color = texture(colorTexture, fragTexcoord);
//	color = vec4(1, 0, 0, 0);
}
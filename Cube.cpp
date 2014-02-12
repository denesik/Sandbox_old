#include "Cube.h"

const float Cube::cubePositions[24][3] = 
{
	{0, 1, 1}, {1, 1, 1}, {1, 0, 1}, {0, 0, 1}, // front
	{1, 1, 0}, {0, 1, 0}, {0, 0, 0}, {1, 0, 0}, // back
	{0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1}, // top
	{1, 0, 0}, {0, 0, 0}, {0, 0, 1}, {1, 0, 1}, // bottom
	{0, 1, 0}, {0, 1, 1}, {0, 0, 1}, {0, 0, 0}, // left
	{1, 1, 1}, {1, 1, 0}, {1, 0, 0}, {1, 0, 1}  // right
};

const float Cube::cubeTexcoords[24][2] = 
{
	{0.0f,1.0f}, {1.0f,1.0f}, {1.0f,0.0f}, {0.0f,0.0f}, // front
	{0.0f,1.0f}, {1.0f,1.0f}, {1.0f,0.0f}, {0.0f,0.0f}, // back
	{0.0f,1.0f}, {1.0f,1.0f}, {1.0f,0.0f}, {0.0f,0.0f}, // top
	{0.0f,1.0f}, {1.0f,1.0f}, {1.0f,0.0f}, {0.0f,0.0f}, // bottom
	{0.0f,1.0f}, {1.0f,1.0f}, {1.0f,0.0f}, {0.0f,0.0f}, // left
	{0.0f,1.0f}, {1.0f,1.0f}, {1.0f,0.0f}, {0.0f,0.0f}  // right
};


Cube::Cube(void)
{

}


Cube::~Cube(void)
{
}

void Cube::GenerateVertexPosition( const vec3 &pos, unsigned int size, float* data )
{
	for(unsigned int i = 0; i < 24; i++)
	{
		data[i * 3]     = cubePositions[i][0] + pos[0];
		data[i * 3 + 1] = cubePositions[i][1] + pos[1];
		data[i * 3 + 2] = cubePositions[i][2] + pos[2];
	}
}

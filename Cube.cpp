#include "Cube.h"

static const float __vertexPositions[24][3] = 
{	// по часовой стрелке
	{0, 0, 0}, {0, 1, 0}, {1, 1, 0}, {1, 0, 0}, // front
	{1, 0,-1}, {1, 1,-1}, {0, 1,-1}, {0, 0,-1}, // back
	{0, 1, 0}, {0, 1,-1}, {1, 1,-1}, {1, 1, 0}, // top
	{0, 0,-1}, {0, 0, 0}, {1, 0, 0}, {1, 0,-1}, // bottom
	{0, 0,-1}, {0, 1,-1}, {0, 1, 0}, {0, 0, 0}, // left
	{1, 0, 0}, {1, 1, 0}, {1, 1,-1}, {1, 0,-1}  // right
};

static const uint32_t __vertexIndex[36] = 
{
	0, 3, 2,  2, 1, 0,  // front
	4, 7, 6,  6, 5, 4,  // back
	8,11, 10, 10,9, 8,  // top
	12,15,14, 14,13,12, // bottom
	16,19,18, 18,17,16, // left
	20,23,22, 22,21,20  // right
};

Cube::Cube(void) : vertexPositionsSize(72), vertexTexcoordsSize(48), vertexIndexSize(36)
{
	vertexTexcoords = new float[vertexTexcoordsSize];
	for(unsigned int i = 0; i < vertexTexcoordsSize / 8; i++)
	{
		vertexTexcoords[i * 8 + 0] = 0;
		vertexTexcoords[i * 8 + 1] = 0;

		vertexTexcoords[i * 8 + 2] = 0;
		vertexTexcoords[i * 8 + 3] = 1;

		vertexTexcoords[i * 8 + 4] = 1;
		vertexTexcoords[i * 8 + 5] = 1;

		vertexTexcoords[i * 8 + 6] = 1;
		vertexTexcoords[i * 8 + 7] = 0;
	}

	vertexPositions = new float[vertexPositionsSize];
	for(unsigned int i = 0; i < vertexPositionsSize / 3; i++)
	{
		vertexPositions[i * 3] = __vertexPositions[i][0];
		vertexPositions[i * 3 + 1] = __vertexPositions[i][1];
		vertexPositions[i * 3 + 2] = __vertexPositions[i][2];
	}


	vertexIndex = new uint32_t[vertexIndexSize];
	for(unsigned int i = 0; i < vertexIndexSize; i++)
	{
		vertexIndex[i] = __vertexIndex[i];
	}

}


Cube::~Cube(void)
{
	delete[] vertexTexcoords;
	vertexTexcoords = nullptr;
	delete[] vertexPositions;
	vertexPositions = nullptr;
	delete[] vertexIndex;
	vertexIndex = nullptr;
}

BufferArray Cube::GetVertexPosition(const vec3 &pos)
{
	for(unsigned int i = 0; i < vertexPositionsSize / 3; i++)
	{
		vertexPositions[i * 3] =     __vertexPositions[i][0] + pos[0];
		vertexPositions[i * 3 + 1] = __vertexPositions[i][1] + pos[1];
		vertexPositions[i * 3 + 2] = __vertexPositions[i][2] + pos[2];
	}
	BufferArray vpa;
	vpa.lenght = vertexPositionsSize;
	vpa.sizeElement = sizeof(float);
	vpa.data = vertexPositions;

	return vpa;
}

void Cube::SetTextureAllSide(const Texture &_texture)
{
	for(unsigned int i = 0; i < vertexTexcoordsSize / 8; i++)
	{
		vertexTexcoords[i * 4 + 0] = _texture.u1;
		vertexTexcoords[i * 4 + 1] = _texture.v1;

		vertexTexcoords[i * 4 + 2] = _texture.u1;
		vertexTexcoords[i * 4 + 3] = _texture.v2;

		vertexTexcoords[i * 4 + 4] = _texture.u2;
		vertexTexcoords[i * 4 + 5] = _texture.v2;

		vertexTexcoords[i * 4 + 6] = _texture.u2;
		vertexTexcoords[i * 4 + 7] = _texture.v1;

		texture[i] = _texture;
	}
}

BufferArray Cube::GetVertexIndex()
{
	BufferArray ba;
	ba.lenght = vertexIndexSize;
	ba.sizeElement = sizeof(uint32_t);
	ba.data = vertexIndex;

	return ba;
}

BufferArray Cube::GetTextureCoord()
{
	BufferArray ba;
	ba.lenght = vertexTexcoordsSize;
	ba.sizeElement = sizeof(float);
	ba.data = vertexTexcoords;

	return ba;
}
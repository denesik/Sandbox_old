#include "Cube.h"

#define VERTEXCOUNT 24
#define INDEXCOUNT 36

static const float __vertexPositions[VERTEXCOUNT][3] = 
{	// по часовой стрелке
	{0, 0, 0}, {0, 1, 0}, {1, 1, 0}, {1, 0, 0}, // front
	{1, 0,-1}, {1, 1,-1}, {0, 1,-1}, {0, 0,-1}, // back
	{0, 1, 0}, {0, 1,-1}, {1, 1,-1}, {1, 1, 0}, // top
	{0, 0,-1}, {0, 0, 0}, {1, 0, 0}, {1, 0,-1}, // bottom
	{0, 0,-1}, {0, 1,-1}, {0, 1, 0}, {0, 0, 0}, // left
	{1, 0, 0}, {1, 1, 0}, {1, 1,-1}, {1, 0,-1}  // right
};

static const uint32_t __vertexIndex[INDEXCOUNT] = 
{
	0, 3, 2,  2, 1, 0,  // front
	4, 7, 6,  6, 5, 4,  // back
	8,11, 10, 10,9, 8,  // top
	12,15,14, 14,13,12, // bottom
	16,19,18, 18,17,16, // left
	20,23,22, 22,21,20  // right
};

Cube::Cube(void)
	: buffer(false, true, false)
{
	buffer.Reserve(VERTEXCOUNT, INDEXCOUNT);
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;

	for(unsigned int i = 0; i < INDEXCOUNT; i++)
	{

		buffer.indexBuffer[i] = __vertexIndex[i];
	}
	buffer.ibSize = INDEXCOUNT;
}


Cube::~Cube(void)
{

}

void Cube::SetTextureAllSide(const Texture &_texture)
{
	for(unsigned int i = 0; i < 6; i++)
	{
		texture[i] = _texture;
	}
}

BufferArray & Cube::GetBufferArray()
{
	for(unsigned int i = 0; i < 6; i++)
	{
		unsigned int j = 0;
		buffer.vertexBuffer[i * 20 + j++] = __vertexPositions[i * 4 + 0][0] + x;	//x
		buffer.vertexBuffer[i * 20 + j++] = __vertexPositions[i * 4 + 0][1] + y;	//y
		buffer.vertexBuffer[i * 20 + j++] = __vertexPositions[i * 4 + 0][2] + z;	//z

		buffer.vertexBuffer[i * 20 + j++] = texture[i].u1;
		buffer.vertexBuffer[i * 20 + j++] = texture[i].v1;

		buffer.vertexBuffer[i * 20 + j++] = __vertexPositions[i * 4 + 1][0] + x;	//x
		buffer.vertexBuffer[i * 20 + j++] = __vertexPositions[i * 4 + 1][1] + y;	//y
		buffer.vertexBuffer[i * 20 + j++] = __vertexPositions[i * 4 + 1][2] + z;	//z

		buffer.vertexBuffer[i * 20 + j++] = texture[i].u1;
		buffer.vertexBuffer[i * 20 + j++] = texture[i].v2;

		buffer.vertexBuffer[i * 20 + j++] = __vertexPositions[i * 4 + 2][0] + x;	//x
		buffer.vertexBuffer[i * 20 + j++] = __vertexPositions[i * 4 + 2][1] + y;	//y
		buffer.vertexBuffer[i * 20 + j++] = __vertexPositions[i * 4 + 2][2] + z;	//z

		buffer.vertexBuffer[i * 20 + j++] = texture[i].u2;
		buffer.vertexBuffer[i * 20 + j++] = texture[i].v2;

		buffer.vertexBuffer[i * 20 + j++] = __vertexPositions[i * 4 + 3][0] + x;	//x
		buffer.vertexBuffer[i * 20 + j++] = __vertexPositions[i * 4 + 3][1] + y;	//y
		buffer.vertexBuffer[i * 20 + j++] = __vertexPositions[i * 4 + 3][2] + z;	//z

		buffer.vertexBuffer[i * 20 + j++] = texture[i].u2;
		buffer.vertexBuffer[i * 20 + j++] = texture[i].v1;

		buffer.ibSize += 20;
	}

	return buffer;
}

void Cube::SetPos( const vec3 &pos )
{
	x = pos[0];
	y = pos[1];
	z = pos[2];
}

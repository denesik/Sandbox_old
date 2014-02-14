#include "Rectangle2D.h"


Rectangle2D::Rectangle2D(void)
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	width = 1.0f;
	height = 1.0f;

	vertexPositions = new float[4 * 3];
	vertexTexcoords = new float[4 * 2];
	vertexIndex = new uint32_t[6];
}


Rectangle2D::~Rectangle2D(void)
{
	delete[] vertexIndex;
	vertexIndex = nullptr;
	delete[] vertexTexcoords;
	vertexTexcoords = nullptr;
	delete[] vertexPositions;
	vertexPositions = nullptr;
}

void Rectangle2D::SetSize( float _width, float _height )
{
	width = _width;
	height = _height;
}

void Rectangle2D::SetPos( const vec2 &_pos )
{
	x = _pos[0];
	y = _pos[1];
}

void Rectangle2D::SetTexture( const Texture &_texture )
{
	texture = texture;
}

BufferArray Rectangle2D::GetVertexPosition()
{
	BufferArray vertexBuffer;

	vertexBuffer.lenght = 4 * 3;
	vertexBuffer.data = vertexPositions;
	vertexBuffer.sizeElement = sizeof(float);

	// низ лево
	vertexPositions[0] = x;
	vertexPositions[1] = y;
	vertexPositions[2] = z;

	// верх лево
	vertexPositions[3] = x;
	vertexPositions[4] = y + height;
	vertexPositions[5] = z;

	//верх право
	vertexPositions[6] = x + width;
	vertexPositions[7] = y + height;
	vertexPositions[8] = z;

	//верх право
	vertexPositions[9] = x + width;
	vertexPositions[10] = y;
	vertexPositions[11] = z;

	return vertexBuffer;
}

BufferArray Rectangle2D::GetTextureCoord()
{
	BufferArray textcoordBuffer;

	textcoordBuffer.lenght = 4 * 2;
	textcoordBuffer.data = vertexTexcoords;
	textcoordBuffer.sizeElement = sizeof(float);

	vertexTexcoords[0] = texture.u1;
	vertexTexcoords[1] = texture.v1;

	vertexTexcoords[2] = texture.u1;
	vertexTexcoords[3] = texture.v2;

	vertexTexcoords[4] = texture.u2;
	vertexTexcoords[5] = texture.v2;

	vertexTexcoords[6] = texture.u2;
	vertexTexcoords[7] = texture.v1;

	return textcoordBuffer;
}

BufferArray Rectangle2D::GetVertexIndex()
{
	BufferArray indexBuffer;

	indexBuffer.lenght =  6;
	indexBuffer.data = vertexIndex;
	indexBuffer.sizeElement = sizeof(uint32_t);

	vertexIndex[0] = 0;
	vertexIndex[1] = 3;
	vertexIndex[2] = 2;

	vertexIndex[3] = 2;
	vertexIndex[4] = 1;
	vertexIndex[5] = 0;

	return indexBuffer;
}

Texture Rectangle2D::GetTexture()
{
	return texture;
}

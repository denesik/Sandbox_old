#include "Rectangle.h"


Rectangle::Rectangle(void)
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	width = 1.0f;
	height = 1.0f;

	bufferArrayVTI.arrayVertex.reserve(4 * 3);
	bufferArrayVTI.arrayTextureCoord.reserve(4 * 2);
	bufferArrayVTI.arrayIndex.reserve(6);

	for(unsigned int i = 0; i < 4 * 3; i++)
	{
		bufferArrayVTI.arrayVertex.push_back(0); 
	}
	for(unsigned int i = 0; i < 4 * 2; i++)
	{
		bufferArrayVTI.arrayTextureCoord.push_back(0); 
	}
	for(unsigned int i = 0; i < 6; i++)
	{
		bufferArrayVTI.arrayIndex.push_back(0); 
	}
}


Rectangle::~Rectangle(void)
{

}

void Rectangle::SetSize( float _width, float _height )
{
	width = _width;
	height = _height;
}

void Rectangle::SetPos( const vec3 &_pos )
{
	x = _pos[0];
	y = _pos[1];
	z = _pos[2];
}

void Rectangle::SetTexture( const Texture &_texture )
{
	texture = _texture;
}

ArrayVertex &Rectangle::GetVertexPosition()
{
	// низ лево
	bufferArrayVTI.arrayVertex[0] = x;
	bufferArrayVTI.arrayVertex[1] = y;
	bufferArrayVTI.arrayVertex[2] = z;

	// верх лево
	bufferArrayVTI.arrayVertex[3] = x;
	bufferArrayVTI.arrayVertex[4] = y + height;
	bufferArrayVTI.arrayVertex[5] = z;

	//верх право
	bufferArrayVTI.arrayVertex[6] = x + width;
	bufferArrayVTI.arrayVertex[7] = y + height;
	bufferArrayVTI.arrayVertex[8] = z;

	//верх право
	bufferArrayVTI.arrayVertex[9] = x + width;
	bufferArrayVTI.arrayVertex[10] = y;
	bufferArrayVTI.arrayVertex[11] = z;

	return bufferArrayVTI.arrayVertex;
}

ArrayTextureCoord &Rectangle::GetTextureCoord()
{
	// в ортогональной проекции текстура будет перевернута по оси 0X
	// текстурные координаты v1, v2 инвертированы
	bufferArrayVTI.arrayTextureCoord[0] = texture.u1;
	bufferArrayVTI.arrayTextureCoord[1] = texture.v2;

	bufferArrayVTI.arrayTextureCoord[2] = texture.u1;
	bufferArrayVTI.arrayTextureCoord[3] = texture.v1;

	bufferArrayVTI.arrayTextureCoord[4] = texture.u2;
	bufferArrayVTI.arrayTextureCoord[5] = texture.v1;

	bufferArrayVTI.arrayTextureCoord[6] = texture.u2;
	bufferArrayVTI.arrayTextureCoord[7] = texture.v2;

	return bufferArrayVTI.arrayTextureCoord;
}

ArrayIndex &Rectangle::GetVertexIndex()
{
	bufferArrayVTI.arrayIndex[0] = 0;
	bufferArrayVTI.arrayIndex[1] = 3;
	bufferArrayVTI.arrayIndex[2] = 2;

	bufferArrayVTI.arrayIndex[3] = 2;
	bufferArrayVTI.arrayIndex[4] = 1;
	bufferArrayVTI.arrayIndex[5] = 0;

	return bufferArrayVTI.arrayIndex;
}

Texture Rectangle::GetTexture()
{
	return texture;
}

BufferArrayVTI &Rectangle::GetBufferArrayVTI()
{
	GetVertexPosition();
	GetTextureCoord();
	GetVertexIndex();

	return bufferArrayVTI;
}

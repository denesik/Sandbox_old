#include "Rectangle.h"


Rectangle::Rectangle(void)
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	width = 1.0f;
	height = 1.0f;

	arrayVertex.reserve(4 * 3);
	arrayTextureCoord.reserve(4 * 2);
	arrayIndex.reserve(6);

	for(unsigned int i = 0; i < 4 * 3; i++)
	{
		arrayVertex.push_back(0); 
	}
	for(unsigned int i = 0; i < 4 * 2; i++)
	{
		arrayTextureCoord.push_back(0); 
	}
	for(unsigned int i = 0; i < 6; i++)
	{
		arrayIndex.push_back(0); 
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
	arrayVertex[0] = x;
	arrayVertex[1] = y;
	arrayVertex[2] = z;

	// верх лево
	arrayVertex[3] = x;
	arrayVertex[4] = y + height;
	arrayVertex[5] = z;

	//верх право
	arrayVertex[6] = x + width;
	arrayVertex[7] = y + height;
	arrayVertex[8] = z;

	//верх право
	arrayVertex[9] = x + width;
	arrayVertex[10] = y;
	arrayVertex[11] = z;

	return arrayVertex;
}

ArrayTextureCoord &Rectangle::GetTextureCoord()
{
	// в ортогональной проекции текстура будет перевернута по оси 0X
	// текстурные координаты v1, v2 инвертированы
	arrayTextureCoord[0] = texture.u1;
	arrayTextureCoord[1] = texture.v2;

	arrayTextureCoord[2] = texture.u1;
	arrayTextureCoord[3] = texture.v1;

	arrayTextureCoord[4] = texture.u2;
	arrayTextureCoord[5] = texture.v1;

	arrayTextureCoord[6] = texture.u2;
	arrayTextureCoord[7] = texture.v2;

	return arrayTextureCoord;
}

ArrayIndex &Rectangle::GetVertexIndex()
{
	arrayIndex[0] = 0;
	arrayIndex[1] = 3;
	arrayIndex[2] = 2;

	arrayIndex[3] = 2;
	arrayIndex[4] = 1;
	arrayIndex[5] = 0;

	return arrayIndex;
}

Texture Rectangle::GetTexture()
{
	return texture;
}

#include "Rectangle.h"


Rectangle::Rectangle(void)
{
	buffer.Create(false, true, false, 4, 6);
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	width = 1.0f;
	height = 1.0f;

	for(unsigned int i = 0; i < 4 * 3 + 4 * 2; i++)
	{
		buffer.vertexBuffer.push_back(0); 
	}
	
	buffer.indexBuffer.push_back(2);
	buffer.indexBuffer.push_back(3);
	buffer.indexBuffer.push_back(0);

	buffer.indexBuffer.push_back(0);
	buffer.indexBuffer.push_back(1);
	buffer.indexBuffer.push_back(2);

	buffer.vbSize = buffer.vertexBuffer.size();
	buffer.ibSize = buffer.indexBuffer.size();
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

Texture Rectangle::GetTexture()
{
	return texture;
}

BufferArray &Rectangle::GetBufferArray()
{
	// в ортогональной проекции текстура будет перевернута по оси 0X
	// текстурные координаты v1, v2 инвертированы
	// 
	// низ лево
	buffer.vertexBuffer[0] = x;
	buffer.vertexBuffer[1] = y;
	buffer.vertexBuffer[2] = z;

	buffer.vertexBuffer[3] = texture.u1;
	buffer.vertexBuffer[4] = texture.v2;

	// верх лево
	buffer.vertexBuffer[5] = x;
	buffer.vertexBuffer[6] = y + height;
	buffer.vertexBuffer[7] = z;

	buffer.vertexBuffer[8] = texture.u1;
	buffer.vertexBuffer[9] = texture.v1;

	//верх право
	buffer.vertexBuffer[10] = x + width;
	buffer.vertexBuffer[11] = y + height;
	buffer.vertexBuffer[12] = z;

	buffer.vertexBuffer[13] = texture.u2;
	buffer.vertexBuffer[14] = texture.v1;

	//верх право
	buffer.vertexBuffer[15] = x + width;
	buffer.vertexBuffer[16] = y;
	buffer.vertexBuffer[17] = z;

	buffer.vertexBuffer[18] = texture.u2;
	buffer.vertexBuffer[19] = texture.v2;

	return buffer;
}

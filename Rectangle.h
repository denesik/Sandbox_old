#ifndef Rectangle_h__
#define Rectangle_h__

#include "GameMath.h"
#include "TextureManager.h"
#include "Render.h"

class Rectangle
{
private:
	Texture texture;

	float x, y, z;
	float width;
	float height;

	BufferArrayVTI bufferArrayVTI;

public:
	Rectangle(void);
	~Rectangle(void);

	void SetSize(float width, float height);
	void SetPos(const vec3 &pos);

	void SetTexture(const Texture &texture);
	Texture GetTexture();

	ArrayVertex &GetVertexPosition();
	ArrayTextureCoord &GetTextureCoord();
	ArrayIndex &GetVertexIndex();

	BufferArrayVTI &GetBufferArrayVTI();

};


#endif // Rectangle_h__
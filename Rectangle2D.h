#ifndef Rectangle2D_h__
#define Rectangle2D_h__

#include "GameMath.h"
#include "TextureManager.h"
#include "Render.h"

class Rectangle2D
{
private:
	Texture texture;

	float x, y, z;
	float width;
	float height;

	float *vertexPositions; // [4][3]
	float *vertexTexcoords; // [4][2]
	// индексы вершин куба в порядке поротив часовой стрелки
	uint32_t *vertexIndex;// [6]

public:
	Rectangle2D(void);
	~Rectangle2D(void);

	void SetSize(float width, float height);
	void SetPos(const vec2 &pos);

	void SetTexture(const Texture &texture);
	Texture GetTexture();

	BufferArray GetVertexPosition();
	BufferArray GetTextureCoord();
	BufferArray GetVertexIndex();

};


#endif // Rectangle2D_h__
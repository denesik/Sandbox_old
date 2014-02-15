#ifndef Rectangle2D_h__
#define Rectangle2D_h__

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

	ArrayVertex arrayVertex;
	ArrayTextureCoord arrayTextureCoord;
	ArrayIndex arrayIndex;

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

};


#endif // Rectangle2D_h__
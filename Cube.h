#ifndef Cube_h__
#define Cube_h__

#include "GameMath.h"
#include "Render.h"
#include "TextureManager.h"


class Cube
{
private:
	BufferArray buffer;

	float x, y, z;
	Texture texture[6];

public:
	Cube(void);
	~Cube(void);

	void SetPos(const vec3 &pos);

	BufferArray &GetBufferArray();

	void SetTextureAllSide(const Texture &texture);

};


#endif // Cube_h__
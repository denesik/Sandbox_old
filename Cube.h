#ifndef Cube_h__
#define Cube_h__

#include "GameMath.h"
#include "Render.h"
#include "TextureManager.h"


class Cube
{
private:
	const unsigned int vertexPositionsSize;
	const unsigned int vertexTexcoordsSize;
	const unsigned int vertexIndexSize;

	float *vertexPositions; // [24][3]
	float *vertexTexcoords; // [24][2]
	// индексы вершин куба в порядке поротив часовой стрелки
	uint32_t *vertexIndex;// [36]

	Texture texture[6];

public:
	Cube(void);
	~Cube(void);

	BufferArray GetVertexPosition(const vec3 &pos);

	BufferArray GetTextureCoord();

	BufferArray GetVertexIndex();

	void SetTextureAllSide(const Texture &texture);

};


#endif // Cube_h__
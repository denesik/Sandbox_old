#ifndef Cube_h__
#define Cube_h__

#include "GameMath.h"
#include "Render.h"


class Cube
{
private:
	static const float cubePositions[24][3];
	static const float cubeTexcoords[24][2];

public:
	Cube(void);
	~Cube(void);

	void GenerateVertexPosition(const vec3 &pos, unsigned int size, float *data);

	float *GetVertexPosition()
	{
		return cubePositions;
	}
	float *GetTextureCoord()
	{
		return cubeTexcoords;
	}

};


#endif // Cube_h__
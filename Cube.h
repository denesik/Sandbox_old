#ifndef Cube_h__
#define Cube_h__

#include "GameMath.h"
#include "Render.h"
#include "TextureManager.h"


class Cube
{
private:
	float *vertexPositions; // [24][3]
	float *vertexTexcoords; // [24][2]
	// ������� ������ ���� � ������� ������� ������� �������
	uint32_t *vertexIndex;// [36]

	Texture texture[6];

public:
	Cube(void);
	~Cube(void);

	BufferArray GetVertexPosition(const vec3 &pos);

	BufferArray GetTextureCoord()
	{
		BufferArray ba;
		ba.lenght = 48;
		ba.sizeElement = sizeof(float);
		ba.data = vertexTexcoords;

		return ba;
	}

	BufferArray GetVertexIndex()
	{
		BufferArray ba;
		ba.lenght = 36;
		ba.sizeElement = sizeof(uint32_t);
		ba.data = vertexIndex;

		return ba;
	}

	void SetTextureAllSide(const Texture &texture);

};


#endif // Cube_h__
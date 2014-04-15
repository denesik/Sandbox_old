#include "Cube.h"

#define VERTEXCOUNT 24
#define INDEXCOUNT 36

static const float __vertexPositions[VERTEXCOUNT][3] = 
{	// по часовой стрелке
	{-0.5, -0.5, -0.5}, {-0.5,  0.5, -0.5}, { 0.5,  0.5, -0.5}, { 0.5, -0.5, -0.5}, // front
	{ 0.5, -0.5,  0.5}, { 0.5,  0.5,  0.5}, {-0.5,  0.5,  0.5}, {-0.5, -0.5,  0.5}, // back
	{-0.5,  0.5, -0.5}, {-0.5,  0.5,  0.5}, { 0.5,  0.5,  0.5}, { 0.5,  0.5, -0.5}, // top
	{-0.5, -0.5,  0.5}, {-0.5, -0.5, -0.5}, { 0.5, -0.5, -0.5}, { 0.5, -0.5,  0.5}, // bottom
	{-0.5, -0.5,  0.5}, {-0.5,  0.5,  0.5}, {-0.5,  0.5, -0.5}, {-0.5, -0.5, -0.5}, // left
	{ 0.5, -0.5, -0.5}, { 0.5,  0.5, -0.5}, { 0.5,  0.5,  0.5}, { 0.5, -0.5,  0.5}  // right
};

static const uint32_t __vertexIndex[INDEXCOUNT] = 
{
	0, 3, 2,  2, 1, 0,  // front
	4, 7, 6,  6, 5, 4,  // back
	8,11, 10, 10,9, 8,  // top
	12,15,14, 14,13,12, // bottom
	16,19,18, 18,17,16, // left
	20,23,22, 22,21,20  // right
};

Cube::Cube(void)
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	for (unsigned int i = 0; i < 24; i++)
	{
		textureCoord[i][0] = 0.0f;
		textureCoord[i][1] = 0.0f;
	}
}


Cube::~Cube(void)
{

}

void Cube::SetPos( const vec3 &pos )
{
	x = pos[0];
	y = pos[1];
	z = pos[2];
}

void Cube::SetTextureAllSide( const ImageAtlas &atlas, std::string texture )
{
	for (unsigned int i = 0; i < 6; i++)
	{
		gm::Rectangle rect = atlas.GetImagePos(texture);
		gm::Size size = atlas.GetSize();

		float u1 = float(rect.Left()) / float(size.width);
		float v1 = float(rect.Bottom()) / float(size.height);
		float u2 = float(rect.Right()) / float(size.width);
		float v2 = float(rect.Top()) / float(size.height);

		textureCoord[i * 4 + 0][0] = u1;
		textureCoord[i * 4 + 0][1] = v1;
		textureCoord[i * 4 + 1][0] = u1;
		textureCoord[i * 4 + 1][1] = v2;
		textureCoord[i * 4 + 2][0] = u2;
		textureCoord[i * 4 + 2][1] = v2;
		textureCoord[i * 4 + 3][0] = u2;
		textureCoord[i * 4 + 3][1] = v1;
	}
}

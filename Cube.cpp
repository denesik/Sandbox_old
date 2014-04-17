#include "Cube.h"

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

void Cube::SetTexture( Texture tex )
{
	texture = tex;
}

Texture & Cube::GetTexture()
{
	return texture;
}

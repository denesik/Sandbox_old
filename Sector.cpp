#include "Sector.h"
#include "Logger.h"



Sector::Sector( void )
{
	for(int x = 0; x < 64; x++)
		for(int z = 0; z < 64; z++)
			for(int y = 0; y < 25; y++)
			{
				map[x][y][z] = 0;

				if(y == 0 || y == 24 || z == 0 || z == 63 || x == 0 || x == 63)
					map[x][y][z] = 1;
			}

}

Sector::~Sector( void )
{

}

void Sector::CreateGeometry()
{
	buffer.ResetIndex();
	buffer.ResetVertex();

	for(int x = 0; x < 64; x++)
		for(int z = 0; z < 64; z++)
			for(int y = 0; y < 25; y++)
			{
				if(map[x][y][z] == 1)
				{
					Cube &graphicData = block.GetGraphicData();
					graphicData.SetPos(glm::vec3(x, y, z));

					unsigned int sizeVertex = buffer.GetSizeVertex();
					graphicData.InsertBackIndex(buffer, sizeVertex);
					graphicData.InsertBackVertex(buffer);

				}
			}

}

void Sector::Draw()
{

	glBindTexture(GL_TEXTURE_2D, block.GetGraphicData().GetTexture().id);

	buffer.CreateIndexBuffer();
	buffer.CreateVertexBuffer();
	buffer.Draw();
}

void Sector::LoadContent()
{
	block.LoadContent();
}

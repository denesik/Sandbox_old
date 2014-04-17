#include "BlockStatic.h"


BlockStatic::BlockStatic(void)
	: textureFileName("textures/img.png"),
	atlas("Block", Bitmap::FORMAT_RGBA, 32, gm::Size(1024, 1024), gm::Size(64 + 32, 64 + 32)
{

}


BlockStatic::~BlockStatic(void)
{
}

Cube &BlockStatic::GetGraphicData()
{
	return geometry;
}

bool BlockStatic::LoadContent()
{
	atlas.Insert(Bitmap(textureFileName), "texture");
	geometry.SetTextureAllSide(atlas, "texture");

	geometry.SetTexture(CreateTexture(*(atlas.GetBitmap()), true, true));

	return true;
}

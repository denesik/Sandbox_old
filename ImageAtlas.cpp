#include "ImageAtlas.h"


ImageAtlas::ImageAtlas(void)
{
	image = nullptr;
}


ImageAtlas::~ImageAtlas(void)
{
	if(image != nullptr)
	{
		delete image;
		image = nullptr;
	}
}

#include "ImageAtlas.h"
#include "Logger.h"


ImageAtlas::ImageAtlas(void)
{
	image = nullptr;
	box = new ElasticBox;
	InitElasticBox();
}


ImageAtlas::~ImageAtlas(void)
{
	Remove();
	if(box)
	{
		delete box;
		box = nullptr;
	}
}

bool ImageAtlas::InsertImage( Bitmap *_image, iRect &rect )
{
	boxTmp = box;
	imageTmp = _image;
	bool result = InsertImage();

	rect.x = xTmp;
	rect.y = yTmp;
	rect.w = _image->GetWidth();
	rect.h = _image->GetHeight();

	return result;
}

bool ImageAtlas::InsertImage()
{
	ElasticBox *boxTmpParent = boxTmp;

	if(boxTmp->surfaceHeight == 0 && boxTmp->surfaceWidth == 0)
	{
		i32vec2 dstPoint;

		xTmp = dstPoint.x = boxTmp->x;
		yTmp = dstPoint.y = boxTmp->y;

		// Копируем всю поверхность
		bool blitResult = image->Blit(&dstPoint, nullptr, imageTmp);
		if(!blitResult)
		{
			//LOG(LOG_ERROR, "TextureAtlas. Image не добавлена.");
			return false;
		}

		unsigned int w1 = boxTmp->surfaceWidth = imageTmp->GetWidth();
		unsigned int h1 = boxTmp->surfaceHeight = imageTmp->GetHeight();

		boxTmp->childrenBoxSmall = new ElasticBox;
		boxTmp->childrenBoxSmall->surfaceHeight = 0;
		boxTmp->childrenBoxSmall->surfaceWidth = 0;
		boxTmp->childrenBoxSmall->childrenBoxBig = nullptr;
		boxTmp->childrenBoxSmall->childrenBoxSmall = nullptr;
		boxTmp->childrenBoxBig = new ElasticBox;
		boxTmp->childrenBoxBig->surfaceHeight = 0;
		boxTmp->childrenBoxBig->surfaceWidth = 0;
		boxTmp->childrenBoxBig->childrenBoxBig = nullptr;
		boxTmp->childrenBoxBig->childrenBoxSmall = nullptr;
		// Правый нижний прямоугольник
		//double d1 = sqrt(double(h1 * h1) + double((boxTmp->width - w1) * (boxTmp->width - w1)));
		// Левый верхний прямоугольник
		//double d2 = sqrt(double((boxTmp->height - h1) * (boxTmp->height - h1)) + double(w1 * w1));
		
		// Правый нижний прямоугольник
		double d1 = sqrt(double(boxTmp->height * boxTmp->height) + double((boxTmp->width - w1 - indent) * (boxTmp->width - w1 - indent)));
		// Левый верхний прямоугольник
		double d2 = sqrt(double((boxTmp->height - h1 - indent) * (boxTmp->height - h1 - indent)) + double(boxTmp->width * boxTmp->width));

		if(d1 < d2)
		{
			// Правый нижний прямоугольник - маленький
			boxTmp->childrenBoxSmall->height = h1;
			boxTmp->childrenBoxSmall->width = boxTmp->width - w1 - indent;
			boxTmp->childrenBoxSmall->x = boxTmp->x + w1 + indent;
			boxTmp->childrenBoxSmall->y = boxTmp->y;

			boxTmp->childrenBoxBig->x = boxTmp->x;
			boxTmp->childrenBoxBig->y = boxTmp->y + h1 + indent;
			boxTmp->childrenBoxBig->width = boxTmp->width;
			boxTmp->childrenBoxBig->height = boxTmp->height - h1 - indent;
		}
		else
		{
			// Левый верхний прямоугольник - маленький
			boxTmp->childrenBoxSmall->height = boxTmp->height - h1 - indent;
			boxTmp->childrenBoxSmall->width = w1;
			boxTmp->childrenBoxSmall->x = boxTmp->x;
			boxTmp->childrenBoxSmall->y = boxTmp->y + h1 + indent;

			boxTmp->childrenBoxBig->x = boxTmp->x + w1 + indent;
			boxTmp->childrenBoxBig->y = boxTmp->y;
			boxTmp->childrenBoxBig->width = boxTmp->width - w1 - indent;
			boxTmp->childrenBoxBig->height = boxTmp->height;
		}
	}
	else
	{
		int w1 = imageTmp->GetWidth();
		int h1 = imageTmp->GetHeight();
		// Попытаемся уместиться в маленькую площадь.
		if(boxTmp->childrenBoxSmall->height >= h1 && boxTmp->childrenBoxSmall->width >= w1)
		{
			boxTmp = boxTmp->childrenBoxSmall;
			if(InsertImage())
			{
				return true;
			}
			else
				boxTmp = boxTmpParent;
		}

		if(boxTmp->childrenBoxBig->height >= h1 && boxTmp->childrenBoxBig->width >= w1)
		{
			boxTmp = boxTmp->childrenBoxBig;
			return InsertImage();
		}
		return false;
	}

	return true;

}

bool ImageAtlas::Create( unsigned int format, unsigned int width, unsigned int height )
{
	if(!box)
	{
		box = new ElasticBox();
		InitElasticBox();
	}
	image = new Bitmap;
	image->Generate(format , width, height, 0x00000000);
	box->x = 0;
	box->y = 0;
	box->width = width;
	box->height = height;
	box->surfaceHeight = 0;
	box->surfaceWidth = 0;
	box->childrenBoxBig = nullptr;
	box->childrenBoxSmall = nullptr;
	return true;
}

void ImageAtlas::Remove()
{
	if(image != nullptr)
	{
		delete image;
		image = nullptr;
	}
	if(box)
	{
		ElasticBox *boxParent = box;
		if(boxParent->childrenBoxBig)
		{
			box = boxParent->childrenBoxBig;
			RemoveElasticBox();
		}
		if(boxParent->childrenBoxSmall)
		{
			box = boxParent->childrenBoxSmall;
			RemoveElasticBox();
		}

		box = boxParent;
		InitElasticBox();
	}
}

void ImageAtlas::RemoveElasticBox()
{
	ElasticBox *boxParent = box;
	if(boxParent->childrenBoxBig)
	{
		box = boxParent->childrenBoxBig;
		RemoveElasticBox();
	}
	if(boxParent->childrenBoxSmall)
	{
		box = boxParent->childrenBoxSmall;
		RemoveElasticBox();
	}

	delete boxParent;
	boxParent = nullptr;
}

void ImageAtlas::InitElasticBox()
{
	box->x = 0;
	box->y = 0;
	box->width = 0;
	box->height = 0;
	box->surfaceHeight = 0;
	box->surfaceWidth = 0;
	box->childrenBoxBig = nullptr;
	box->childrenBoxSmall = nullptr;
}

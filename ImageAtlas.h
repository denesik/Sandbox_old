#ifndef ImageAtlas_h__
#define ImageAtlas_h__

#include "Bitmap.h"

class ImageAtlas
{
private:

	static const int indent = 1;
	struct ElasticBox
	{
		int surfaceWidth, surfaceHeight;		// Ўирина и высота "€щика"
		int x, y, width, height;				// √абариты "контейнера"
		ElasticBox *childrenBoxSmall;
		ElasticBox *childrenBoxBig;
	};

	Bitmap *image;
	ElasticBox *box;

	Bitmap *imageTmp;
	ElasticBox *boxTmp;

	unsigned int xTmp;
	unsigned int yTmp;

public:
	ImageAtlas(void);
	~ImageAtlas(void);

	bool InsertImage(Bitmap *image, iRect &rect);

	bool Create(unsigned int format, unsigned int width, unsigned int height);

	void Remove();

	Bitmap *GetAtlas()
	{
		return image;
	}

private:
	bool InsertImage();
	void RemoveElasticBox();
	void InitElasticBox();
};


#endif // ImageAtlas_h__
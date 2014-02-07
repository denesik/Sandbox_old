#ifndef TextureManager_h__
#define TextureManager_h__

#include "Bitmap.h"

struct Texture
{
	unsigned int textureId;
	double u1;
	double v1;
	double u2;
	double v2;
};


unsigned int GenerateOpenglBitmap(Bitmap &bitmap, bool smoothing);

void GenerateTextCoord(Bitmap *bitmap, iRect *rect, Texture &texture);

class TextureManager
{
public:
	TextureManager(void);
	~TextureManager(void);

};


#endif // TextureManager_h__

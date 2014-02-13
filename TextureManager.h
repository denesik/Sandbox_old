#ifndef TextureManager_h__
#define TextureManager_h__

#include "Bitmap.h"

struct Texture
{
	unsigned int textureId;
	float u1;
	float v1;
	float u2;
	float v2;
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

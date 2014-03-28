#ifndef Bitmap_h__
#define Bitmap_h__

#include <string>
#include "GameMath.h"

extern unsigned int GetChannelCount(unsigned int format);
extern unsigned int IsAvailableAlpha(unsigned int format);

typedef unsigned char byte;

class Bitmap
{
public:

	enum PixelFormat
	{
		FORMAT_NULL,
		FORMAT_LUMINANCE,
		FORMAT_LUMINANCE_ALPHA,
		FORMAT_RGB,
		FORMAT_RGBA,
	};

private:
	byte *data;
	unsigned int width;
	unsigned int height;
	unsigned int format;

public:
	Bitmap(void);
	~Bitmap(void);

	byte *GetData()
	{
		return data;
	}
	unsigned int GetWidth()
	{
		return width;
	}
	unsigned int GetHeight()
	{
		return height;
	}
	unsigned int GetFormat()
	{
		return format;
	}

	void ConvertFormat(unsigned int format);

	// Вставляем изображение с областью srcrect
	bool Blit(i32vec2 *point, iRect *srcrect, Bitmap *bitmap);

	void Free();

	void Change(unsigned int format, unsigned int width, unsigned int height, byte *data);

	void Generate(unsigned int format, unsigned int width, unsigned int height, unsigned int color);

	bool Load(std::string fileName);
	bool Save(std::string fileName);
};


#endif // Bitmap_h__
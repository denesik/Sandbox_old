#ifndef Bitmap_h__
#define Bitmap_h__

#include <string>

class Bitmap
{
public:

	enum e_pixel_format
	{
		FORMAT_NULL,
		FORMAT_LUMINANCE,
		FORMAT_ALPHA,
		FORMAT_LUMINANCE_ALPHA,
		FORMAT_RGB,
		FORMAT_RGBA,
	};

private:
	unsigned char *data;
	unsigned int width;
	unsigned int height;
	unsigned int format;

public:
	Bitmap(void);
	~Bitmap(void);

	unsigned char *GetData()
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

	void Change(unsigned int format, unsigned int width, unsigned int height, unsigned char *data);

	bool load(std::string fileName);
	bool save(std::string fileName);
};


#endif // Bitmap_h__
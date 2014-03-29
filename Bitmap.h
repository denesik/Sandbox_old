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


class Bitmap1
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
	gm::Size size;
	PixelFormat format;

public:
	// Создаем битмап заданного размера и цвета
	Bitmap1(PixelFormat format, gm::Size &size, unsigned int color);
	
	// Создаем битмап и загружаем данные из файла
	Bitmap1(std::string fileName);
	
	// Создаем копию битмапа
	Bitmap1(const Bitmap1 &bitmap);
	~Bitmap1();

	void Save(std::string fileName);

	int GetHeight();
	int GetWidth();

	gm::Size &GetSize();

	// unsigned int color GetPixel(const gm::Point &pos);
	// SetPixel(const gm::Point &pos, unsigned int color);
	
	// Вставляем битмап
	bool Insert(const Bitmap1 &bitmap);
	bool Insert(const Bitmap1 &bitmap, const gm::Point &pos);
	bool Insert(const Bitmap1 &bitmap, const gm::Point &pos, const gm::Rectangle &rect);

	// Увеличиваем размер битмапа. Если новый размер больше, то заполнить пикселы цветом color
	void Inflate(const gm::Size &size, unsigned int color);
	
	// byte *GetData();

};

#endif // Bitmap_h__
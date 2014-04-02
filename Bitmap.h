#ifndef Bitmap_h__
#define Bitmap_h__

#include <string>
#include "GameMath.h"

using namespace glm;

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
	glm::uint8 *data;
	gm::Size size;
	PixelFormat format;
	unsigned int isAlpha;
	unsigned int stride;

public:

	Bitmap1();

	// Создаем битмап заданного размера и цвета
	Bitmap1(PixelFormat format, const gm::Size &size, const gm::Color &color = gm::Color());

	// Создаем битмап и загружаем данные из файла
	Bitmap1(std::string fileName);
	Bitmap1(const Bitmap1 &bitmap);
	~Bitmap1();

	bool Save(std::string fileName) const;

	int GetHeight() const;
	int GetWidth() const;

	const gm::Size &GetSize() const;

	gm::Color GetPixel(const gm::Point &pos) const;
	void SetPixel(const gm::Point &pos, const gm::Color &color);
	
	PixelFormat GetFormat();

	// Вставляем битмап
	gm::Rectangle Insert(const Bitmap1 &bitmap, const gm::Point &pos = gm::Point());
	gm::Rectangle Insert(const Bitmap1 &bitmap, const gm::Rectangle &rect, const gm::Point &pos = gm::Point());

	// Увеличиваем размер битмапа. Если новый размер больше, то заполнить пикселы цветом color
	void Inflate(const gm::Size &size, const gm::Color &color = gm::Color());
	// Изменяем размер битмапа. Новый размер равен size
	void Resize(const gm::Size &size, const gm::Color &color = gm::Color());
};

#endif // Bitmap_h__
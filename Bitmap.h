#ifndef Bitmap_h__
#define Bitmap_h__

#include <string>
#include "GameMath.h"

using namespace glm;

extern unsigned int GetChannelCount(unsigned int format);
extern unsigned int IsAvailableAlpha(unsigned int format);

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
	glm::uint8 *data;
	gm::Size size;
	PixelFormat format;
	unsigned int isAlpha;
	unsigned int stride;

public:

	Bitmap();

	// ������� ������ ��������� ������� � �����
	Bitmap(PixelFormat format, const gm::Size &size, const gm::Color &color = gm::Color());

	// ������� ������ � ��������� ������ �� �����
	Bitmap(std::string fileName);
	Bitmap(const Bitmap &bitmap);
	~Bitmap();

	bool Save(std::string fileName) const;

	int GetHeight() const;
	int GetWidth() const;

	const gm::Size &GetSize() const;

	gm::Color GetPixel(const gm::Point &pos) const;
	void SetPixel(const gm::Point &pos, const gm::Color &color);
	
	PixelFormat GetFormat();

	// ��������� ������
	gm::Rectangle Insert(const Bitmap &bitmap, const gm::Point &pos = gm::Point());
	gm::Rectangle Insert(const Bitmap &bitmap, const gm::Rectangle &rect, const gm::Point &pos = gm::Point());

	// ����������� ������ �������. ���� ����� ������ ������, �� ��������� ������� ������ color
	void Inflate(const gm::Size &size, const gm::Color &color = gm::Color());
	// �������� ������ �������. ����� ������ ����� size
	void Resize(const gm::Size &size, const gm::Color &color = gm::Color());

	glm::uint8 *GetData();
};

#endif // Bitmap_h__
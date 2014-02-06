#include "TextureManager.h"
#include <glfw3.h>
#include "Logger.h"


unsigned int GenerateOpenglBitmap(Bitmap &bitmap, bool smoothing)
{
	unsigned int glBitmap = 0;
	glGenTextures(1, &glBitmap);
	glBindTexture(GL_TEXTURE_2D, glBitmap);

	GLint param = GL_LINEAR;
	if(!smoothing) param = GL_NEAREST;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);

	unsigned int format = bitmap.GetFormat();
	unsigned int colorType = GL_RGB;
	switch (format)
	{
	case Bitmap::FORMAT_LUMINANCE:
		{
			colorType = GL_LUMINANCE;
			break;
		}

	case Bitmap::FORMAT_LUMINANCE_ALPHA:
		{
			colorType = GL_LUMINANCE_ALPHA;
			break;
		}

	case Bitmap::FORMAT_RGB:
		{
			colorType = GL_RGB;
			break;
		}

	case Bitmap::FORMAT_RGBA:
		{
			colorType = GL_RGBA;
			break;
		}

	default:
		{
			LOG(LOG_WARNING, "Generate GLBitmap. �� �������������� ��� �����.");
			break;
		}
	}

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, colorType, bitmap.GetWidth(), bitmap.GetHeight(), 0, colorType, GL_UNSIGNED_BYTE, bitmap.GetData());

	return glBitmap;
}

void GenerateTextCoord( Bitmap *bitmap, iRect *rect, Texture &texture )
{
	unsigned int width = bitmap->GetWidth();
	unsigned int height = bitmap->GetHeight();

	if(rect == nullptr)
	{
		texture.u1 = 0;
		texture.v1 = 0;
		texture.u2 = 1;
		texture.v2 = 1;
		return;
	}

	double kx = 1.0 / double(width);
	double ky = 1.0 / double(height);

	texture.u1 = kx * double(rect->x);
	texture.v1 = ky * double(rect->y);
	texture.u2 = kx * double(rect->x + rect->w);
	texture.v2 = ky * double(rect->y + rect->h);

}

TextureManager::TextureManager(void)
{
}


TextureManager::~TextureManager(void)
{
}

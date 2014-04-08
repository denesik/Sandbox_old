#include "Bitmap.h"
#include "Logger.h"

#include <cstdio>
#include <png.h>


unsigned int GetChannelCount(unsigned int format)
{
	switch (format)
	{
	case Bitmap::FORMAT_LUMINANCE:
		return 1;

	case Bitmap::FORMAT_LUMINANCE_ALPHA:
		return 2;

	case Bitmap::FORMAT_RGB:
		return 3;

	case Bitmap::FORMAT_RGBA:
		return 4;
	}

	return 0;
}

unsigned int IsAvailableAlpha(unsigned int format)
{
	switch (format)
	{
	case Bitmap::FORMAT_LUMINANCE_ALPHA:
	case Bitmap::FORMAT_RGBA:
		return 1;

	}

	return 0;
}

static bool LoadBMP(Bitmap &bitmap, FILE *file)
{

	png_struct* png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png)
	{
		//LOG(LOG_WARNING, "Libng. Невозможно создать png структуру.");
		return false;
	}

	png_info* info = png_create_info_struct(png);

	if (!info)
	{
		png_destroy_read_struct(&png, NULL, NULL);
		//LOG(LOG_WARNING, "Libng. Невозможно создать png info структуру.");
		return false;
	}

	png_byte* data = nullptr;

	if (setjmp(png_jmpbuf(png)))
	{

		png_destroy_read_struct(&png, &info, NULL);
		return false;
	}

	png_init_io(png, file);
	png_read_info(png, info);

	switch (png_get_color_type(png, info))
	{
	case PNG_COLOR_TYPE_PALETTE: // color index to RGB
		{
			png_set_palette_to_rgb(png);
			break;
		}

	case PNG_COLOR_TYPE_GRAY: // 1, 2, 4 bits grayscale to 8 bit grayscale
		{
			if (png_get_bit_depth(png, info) < 8)
			{
				png_set_expand_gray_1_2_4_to_8(png);
			}

			break;
		}

	}

	// transparency to alpha channel
	if (png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);

	// 16 bit per channel to 8 bit per channel
	if (png_get_bit_depth(png, info) == 16)
		png_set_strip_16(png);

	unsigned int pass_count = 1;

	if (png_get_interlace_type(png, info))
		pass_count = png_set_interlace_handling(png);

	png_read_update_info(png, info);
	unsigned int format = Bitmap::FORMAT_NULL;
	unsigned int width = png_get_image_width(png, info);
	unsigned int height = png_get_image_height(png, info);
	unsigned int row_length = png_get_rowbytes(png, info);

	switch (png_get_color_type(png, info))
	{
	case PNG_COLOR_TYPE_GRAY:
		{
			format = Bitmap::FORMAT_LUMINANCE;
			break;
		}

	case PNG_COLOR_TYPE_GRAY_ALPHA:
		{
			format = Bitmap::FORMAT_LUMINANCE_ALPHA;
			break;
		}

	case PNG_COLOR_TYPE_RGB:
		{
			format = Bitmap::FORMAT_RGB;
			break;
		}

	case PNG_COLOR_TYPE_RGB_ALPHA:
		{
			format = Bitmap::FORMAT_RGBA;
			break;
		}
	}

	data = new png_byte[row_length * height];

	for (unsigned int i = 0; i < pass_count; i++)
	{
		unsigned int offset = row_length * (height - 1);

		for(unsigned int j = 0; j < height; j++)
		{
			png_read_row(png, data + offset, NULL);
			offset -= row_length;
		}
	}

	png_destroy_read_struct(&png, &info, NULL);
	bitmap.Change(format, width, height, data);

	return true; 
}

static bool SaveBMP(Bitmap &bitmap, FILE *file)
{
	png_struct* png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png)
	{
		//LOG(LOG_WARNING, "Libng. Невозможно создать png структуру.");
		return false;
	}

	png_info* info = png_create_info_struct(png);

	if (!info)
	{
		png_destroy_write_struct(&png, NULL);
		//LOG(LOG_WARNING, "Libng. Невозможно создать png info структуру.");
		return false;
	}

	if (setjmp(png_jmpbuf(png)))
	{
		png_destroy_write_struct(&png, &info);
		return false;
	}

	png_init_io(png, file);
	unsigned int format = bitmap.GetFormat();
	unsigned int width = bitmap.GetWidth();
	unsigned int height = bitmap.GetHeight();
	unsigned int channel_count = GetChannelCount(format);
	unsigned int color_type;

	switch (format)
	{
	case Bitmap::FORMAT_LUMINANCE:
		{
			color_type = PNG_COLOR_TYPE_GRAY;
			break;
		}

	case Bitmap::FORMAT_LUMINANCE_ALPHA:
		{
			color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
			break;
		}

	case Bitmap::FORMAT_RGB:
		{
			color_type = PNG_COLOR_TYPE_RGB;
			break;
		}

	case Bitmap::FORMAT_RGBA:
		{
			color_type = PNG_COLOR_TYPE_RGB_ALPHA;
			break;
		}

	default:
		{
			//LOG(LOG_WARNING, "Libng. неподдерживаемый тип цвета.");
			longjmp(png_jmpbuf(png), 1);
			break;
		}
	}

	png_set_IHDR(png, info, width, height, 8, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png, info);
	unsigned int pass_count = 1;

	if (png_get_interlace_type(png, info))
		pass_count = png_set_interlace_handling(png);

	unsigned int row_length = channel_count * width;

	for (unsigned int i = 0; i < pass_count; i++)
	{
		unsigned int offset = row_length * (height - 1);

		for (unsigned int j = 0; j < height; j++)
		{
			png_write_row(png, (png_byte*)bitmap.GetData() + offset);
			offset -= row_length;
		}
	}

	png_write_end(png, NULL);
	png_destroy_write_struct(&png, &info);

	return true;
}

static byte *FormatLuminanceToAny(unsigned int formatOld, unsigned int formatNew, unsigned int width, unsigned int height, byte *data)
{
	unsigned int channelCount = GetChannelCount(formatNew);

	byte *dataNew = new byte[width * height * channelCount];

	unsigned int channelAlpha = (IsAvailableAlpha(formatNew)) ? 1 : 0;

	unsigned int isAlphaOld = IsAvailableAlpha(formatOld);

	for(unsigned int i = 0; i < height * width; i++)
	{
		for (unsigned int k = 0; k < channelCount - channelAlpha; k++)
		{
			dataNew[i * channelCount + k] = (isAlphaOld == 1) ? data[i * 2] : data[i];
		}
		if(channelAlpha > 0)
			dataNew[i * channelCount + channelCount - 1] = (isAlphaOld == 1) ? data[i * 2 + 1] : 255;
	}

	return dataNew;
}

static byte *FormatRGBToAny(unsigned int formatOld, unsigned int formatNew, unsigned int width, unsigned int height, byte *data)
{
	unsigned int channelCountNew = GetChannelCount(formatNew);
	unsigned int channelAlphaNew = (IsAvailableAlpha(formatNew)) ? 1 : 0;

	unsigned int isAlphaOld = IsAvailableAlpha(formatOld);
	unsigned int offsetOld = (isAlphaOld == 1) ? 4 : 3;


	byte *dataNew = new byte[width * height * channelCountNew];

	for(unsigned int i = 0; i < height * width; i++)
	{
		for (unsigned int k = 0; k < channelCountNew - channelAlphaNew; k++)
		{
			if(channelCountNew - channelAlphaNew > 1)
			{
				dataNew[i * channelCountNew + k] = data[i * offsetOld + k];
			}
			else
			{
				dataNew[i * channelCountNew + k] = (data[i * offsetOld] + data[i * offsetOld + 1] + data[i * offsetOld + 2]) / 3;
			}

		}
		if(channelAlphaNew > 0)
			dataNew[i * channelCountNew + channelCountNew - 1] = (isAlphaOld == 1) ? data[i * offsetOld + offsetOld - 1] : 255;
	}

	return dataNew;
}


Bitmap::Bitmap(void)
{
	data = nullptr;
	width = 0;
	height = 0;
	format = FORMAT_NULL;
}

Bitmap::~Bitmap(void)
{
	Free();
}


void Bitmap::Free()
{
	if(data)
	{
		delete[] data;
		data = nullptr;
	}
	width = 0;
	height = 0;
	format = FORMAT_NULL;
}


void Bitmap::Change(unsigned int format_, unsigned int width_, unsigned int height_, byte *data_)
{
	data = data_;
	width = width_;
	height = height_;
	format = format_;
}

#pragma warning(push)
#pragma warning (disable: 4996)

bool Bitmap::Load( std::string fileName )
{
	FILE *file = fopen(fileName.c_str(), "rb");
	if (file == NULL) 
	{
		//LOG(LOG_WARNING, "Bitmap. Невозможно открыть файл " + fileName + ".");
		return false;
	}

	if(LoadBMP(*this, file))
	{
		fclose(file);
		return true;
	}

	fclose(file);
	//LOG(LOG_WARNING, "Bitmap. Файл " + fileName + " не загружен.");
	return false;
}

bool Bitmap::Save( std::string fileName )
{
	FILE *file = fopen(fileName.c_str(), "wb");
	if (!file) 
	{
		//LOG(LOG_WARNING, "Bitmap. Невозможно открыть файл " + fileName + ".");
		return false;
	}

	if(SaveBMP(*this, file))
	{
		fclose(file);
		return true;
	}

	fclose(file);
	//LOG(LOG_WARNING, "Bitmap. Файл " + fileName + " не загружен.");
	return false;
}

#pragma warning (pop)

void Bitmap::ConvertFormat( unsigned int formatNew )
{
	if(formatNew == format)
		return;

	if(formatNew == Bitmap::FORMAT_NULL)
		return;

	byte *dataNew = nullptr;

	switch (format)
	{
	case Bitmap::FORMAT_LUMINANCE:
	case Bitmap::FORMAT_LUMINANCE_ALPHA:
		{
			dataNew = FormatLuminanceToAny(format, formatNew, width, height, data);
			if(data)
			{
				delete[] data;
			}
			data = dataNew;
			break;
		}

	case Bitmap::FORMAT_RGB:
	case Bitmap::FORMAT_RGBA:
		{
			dataNew = FormatRGBToAny(format, formatNew, width, height, data);
			if(data)
			{
				delete[] data;
			}
			data = dataNew;
			break;
		}

	default:
		{
			return;
		}
	}

	format = formatNew;
}


bool Bitmap::Blit( i32vec2 *point, iRect *srcrect, Bitmap *bitmap )
{

	unsigned int channelCount = GetChannelCount(format);

	if( channelCount != GetChannelCount(bitmap->GetFormat()))
		return false;

	iRect srcBitmapRect;
	iRect dstBitmapRect;
	if(srcrect == nullptr)
	{
		srcBitmapRect.h = bitmap->GetHeight();
		srcBitmapRect.w = bitmap->GetWidth();
	}
	else
	{
		srcBitmapRect = *srcrect;
		int offsetX = srcBitmapRect.x + srcBitmapRect.w - bitmap->GetWidth();
		int offsetY = srcBitmapRect.y + srcBitmapRect.h - bitmap->GetHeight();
		if( offsetX > 0)
			srcBitmapRect.w -= offsetX;
		if( offsetY > 0)
			srcBitmapRect.h -= offsetY;

	}

	if(point != nullptr)
	{
		dstBitmapRect.x = point->x;
		dstBitmapRect.y = point->y;
	}

	dstBitmapRect.w = srcBitmapRect.w;
	dstBitmapRect.h = srcBitmapRect.h;

	int offsetX = dstBitmapRect.x + dstBitmapRect.w - width;
	int offsetY = dstBitmapRect.y + dstBitmapRect.h - height;
	if( offsetX > 0)
	{
		dstBitmapRect.w -= offsetX;
		srcBitmapRect.w -= offsetX;
	}
	if( offsetY > 0)
	{
		dstBitmapRect.h -= offsetY;
		srcBitmapRect.h -= offsetY;
	}

	byte *srcData = bitmap->GetData();

	unsigned int srcWidth = bitmap->GetWidth();

	for (unsigned int yy = 0; yy < unsigned int(dstBitmapRect.h); yy++)
		for (unsigned int xx = 0; xx < unsigned int(dstBitmapRect.w); xx++)
		{
			for(unsigned int k = 0; k < channelCount; k++)
			{

				//yy + dstBitmapRect.y	// номер строки

				//(yy + dstBitmapRect.y) * width * channelCount // первый бит в yy строке
				//(xx + dstBitmapRect.x) * channelCount + k // номер бита в строке

				data[((yy + dstBitmapRect.y) * width + (xx + dstBitmapRect.x)) * channelCount + k]
				= srcData[((yy + srcBitmapRect.y) * srcWidth + (xx + srcBitmapRect.x)) * channelCount + k];

			}
		}

		if(srcrect != nullptr)
			*srcrect = dstBitmapRect;

		return true;
}

void Bitmap::Generate( unsigned int format_, unsigned int width_, unsigned int height_, unsigned int color)
{
	format = format_;
	width = width_;
	height = height_;

	unsigned char colorRBG[3];
	colorRBG[0] = 0x000000FF & (color >> 24);
	colorRBG[1] = 0x000000FF & (color >> 16);
	colorRBG[2] = 0x000000FF & (color >> 8);
	unsigned char colorA = 0x000000FF & color;

	unsigned char colorL = (colorRBG[0] + colorRBG[1] + colorRBG[2]) / 3;

	unsigned int alpha = (IsAvailableAlpha(format)) ? 1 : 0;

	unsigned int channelCount = GetChannelCount(format);

	data = new byte[width * height * channelCount];


	for(unsigned int i = 0; i < width * height; i++)
	{
		if( (channelCount - alpha) < 3 )
		{
			data[i * channelCount] = colorL;
		}
		else
		{
			for(unsigned int k = 0; k < channelCount - alpha; k++)
			{
				data[i * channelCount + k] = colorRBG[k];
			}
		}
		if( alpha == 1)
			data[i * channelCount + channelCount - 1] = colorA;
	}

}


/// new ===================================================================


static glm::uint8 *LoadBMP(Bitmap1::PixelFormat &format, gm::Size &size, FILE *file)
{
	glm::uint8 *data = nullptr;
	size.width = 0;
	size.height = 0;
	format = Bitmap1::FORMAT_NULL;

	png_struct* png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png)
	{
		LOG_ERROR("Libng. Невозможно создать png структуру.");
		return false;
	}

	png_info* info = png_create_info_struct(png);

	if (!info)
	{
		png_destroy_read_struct(&png, NULL, NULL);
		LOG_ERROR("Libng. Невозможно создать png info структуру.");
		return false;
	}

	if (setjmp(png_jmpbuf(png)))
	{

		png_destroy_read_struct(&png, &info, NULL);
		return false;
	}

	png_init_io(png, file);
	png_read_info(png, info);

	switch (png_get_color_type(png, info))
	{
	case PNG_COLOR_TYPE_PALETTE: // color index to RGB
		{
			png_set_palette_to_rgb(png);
			break;
		}

	case PNG_COLOR_TYPE_GRAY: // 1, 2, 4 bits grayscale to 8 bit grayscale
		{
			if (png_get_bit_depth(png, info) < 8)
			{
				png_set_expand_gray_1_2_4_to_8(png);
			}

			break;
		}

	}

	// transparency to alpha channel
	if (png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);

	// 16 bit per channel to 8 bit per channel
	if (png_get_bit_depth(png, info) == 16)
		png_set_strip_16(png);

	unsigned int pass_count = 1;

	if (png_get_interlace_type(png, info))
		pass_count = png_set_interlace_handling(png);

	png_read_update_info(png, info);
	size.width = png_get_image_width(png, info);
	size.height = png_get_image_height(png, info);
	int row_length = png_get_rowbytes(png, info);

	switch (png_get_color_type(png, info))
	{
	case PNG_COLOR_TYPE_GRAY:
		{
			format = Bitmap1::FORMAT_LUMINANCE;
			break;
		}

	case PNG_COLOR_TYPE_GRAY_ALPHA:
		{
			format = Bitmap1::FORMAT_LUMINANCE_ALPHA;
			break;
		}

	case PNG_COLOR_TYPE_RGB:
		{
			format = Bitmap1::FORMAT_RGB;
			break;
		}

	case PNG_COLOR_TYPE_RGB_ALPHA:
		{
			format = Bitmap1::FORMAT_RGBA;
			break;
		}
	}

	data = new png_byte[row_length * size.height];

	for (unsigned int i = 0; i < pass_count; i++)
	{
		int offset = (row_length * (size.height - 1)) * i;

		for(int j = 0; j < size.height; j++)
		{
			png_read_row(png, data + offset, NULL);
			offset += row_length;
		}
	}

	png_destroy_read_struct(&png, &info, NULL);

	return data; 
}

static bool SaveBMP(Bitmap1::PixelFormat format, const gm::Size &size, const glm::uint8 *data, FILE *file)
{
	if(size.width < 0 || size.height < 0)
	{
		LOG_ERROR("Savepng. Некорректный размер изображения.");
		return false;
	}
	png_struct* png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png)
	{
		LOG_ERROR("Libpng. Невозможно создать png структуру.");
		return false;
	}

	png_info* info = png_create_info_struct(png);

	if (!info)
	{
		png_destroy_write_struct(&png, NULL);
		LOG_ERROR("Libpng. Невозможно создать png info структуру.");
		return false;
	}

	if (setjmp(png_jmpbuf(png)))
	{
		png_destroy_write_struct(&png, &info);
		return false;
	}

	png_init_io(png, file);
	unsigned int channel_count = GetChannelCount(format);
	unsigned int color_type;

	switch (format)
	{
	case Bitmap::FORMAT_LUMINANCE:
		{
			color_type = PNG_COLOR_TYPE_GRAY;
			break;
		}

	case Bitmap::FORMAT_LUMINANCE_ALPHA:
		{
			color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
			break;
		}

	case Bitmap::FORMAT_RGB:
		{
			color_type = PNG_COLOR_TYPE_RGB;
			break;
		}

	case Bitmap::FORMAT_RGBA:
		{
			color_type = PNG_COLOR_TYPE_RGB_ALPHA;
			break;
		}

	default:
		{
			LOG_WARNING("Libpng. Неподдерживаемый тип цвета.");
			longjmp(png_jmpbuf(png), 1);
			break;
		}
	}

	png_set_IHDR(png, info, size.width, size.height, 8, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png, info);
	unsigned int pass_count = 1;

	if (png_get_interlace_type(png, info))
		pass_count = png_set_interlace_handling(png);

	unsigned int row_length = channel_count * size.width;

	for (unsigned int i = 0; i < pass_count; i++)
	{
		int offset = (row_length * (size.height - 1)) * i;

		for (int j = 0; j < size.height; j++)
		{
			png_write_row(png, (png_byte*)data + offset);
			offset += row_length;
		}
	}

	png_write_end(png, NULL);
	png_destroy_write_struct(&png, &info);

	return true;
}



Bitmap1::Bitmap1()
{
	data = nullptr;
	format = FORMAT_RGBA;
	stride = GetChannelCount(format);
	isAlpha = IsAvailableAlpha(format);
}

Bitmap1::Bitmap1( PixelFormat _format, const gm::Size &_size, const gm::Color &color /*= gm::Color()*/ )
{
	format = _format;
	stride = GetChannelCount(format);
	isAlpha = IsAvailableAlpha(format);
	size = _size;

	data = new glm::uint8[size.width * size.height * stride];

	for(int i = 0; i < size.height; i++)
		for(int j = 0; j < size.width; j++)
		{
			SetPixel(gm::Point(j, i), color);
		}
}

#pragma warning(push)
#pragma warning (disable: 4996)
Bitmap1::Bitmap1( std::string fileName )
{
	data = nullptr;
	format = FORMAT_NULL;	

	FILE *file = fopen(fileName.c_str(), "rb");
	if (file == NULL) 
	{
		LOG_ERROR("Bitmap. Невозможно открыть файл %s.", fileName.c_str());
		return;
	}

	data = LoadBMP(format, size, file);
	fclose(file);
	stride = GetChannelCount(format);
	isAlpha = IsAvailableAlpha(format);

	if(!data)
	{
		LOG_ERROR("Bitmap. Файл %s не загружен.", fileName.c_str());
	}
}
#pragma warning(pop)

Bitmap1::Bitmap1( const Bitmap1 &bitmap )
{
	format = bitmap.format;
	size = bitmap.size;
	stride = bitmap.stride;
	isAlpha = bitmap.isAlpha;

	data = new glm::uint8[size.width * size.height * stride];
	memcpy(data, bitmap.data, size.width * size.height * stride);
}

Bitmap1::~Bitmap1()
{
	if(data)
		delete[] data;
}

#pragma warning(push)
#pragma warning (disable: 4996)
bool Bitmap1::Save( std::string fileName ) const
{
	FILE *file = fopen(fileName.c_str(), "wb");
	if (!file) 
	{
		LOG_ERROR("Bitmap. Невозможно открыть файл %s.", fileName.c_str());
		return false;
	}

	if(SaveBMP(format, size, data, file))
	{
		fclose(file);
		return true;
	}

	fclose(file);
	LOG_ERROR("Bitmap. Файл %s не сохранен.", fileName.c_str());
	return false;
}
#pragma warning(pop)

int Bitmap1::GetHeight() const
{
	return size.height;
}

int Bitmap1::GetWidth() const
{
	return size.width;
}

const gm::Size & Bitmap1::GetSize() const
{
	return size;
}

gm::Color Bitmap1::GetPixel( const gm::Point &pos ) const
{	
	// Надо бы проверку позиции

	gm::Color color;
	glm::uint32 bytePos = pos.y * size.width * stride + pos.x * stride;

	if(isAlpha)
	{
		color.A = data[bytePos + stride - 1];
	}

	// rgb || rgba
	if(stride - isAlpha == 3)
	{
		color.R = data[bytePos];
		color.G = data[bytePos + 1];
		color.B = data[bytePos + 2];
	}
	else
		if(stride - isAlpha == 1)
		{
			color.R = data[bytePos];
			color.G = data[bytePos];
			color.B = data[bytePos];
		}

	return color;
}

void Bitmap1::SetPixel( const gm::Point &pos, const gm::Color &color )
{
	glm::uint32 bytePos = pos.y * size.width * stride + pos.x * stride;

	if(isAlpha)
	{
		data[bytePos + stride - 1] = color.A;
	}

	// rgb || rgba
	if(stride - isAlpha == 3)
	{
		data[bytePos] = color.R;
		data[bytePos + 1] = color.G;
		data[bytePos + 2] = color.B;
	}
	else
		if(stride - isAlpha == 1)
		{
			data[bytePos] = (color.R + color.G + color.B) / 3;
		}
}

gm::Rectangle Bitmap1::Insert( const Bitmap1 &bitmap, const gm::Point &pos )
{
	gm::Rectangle iRect;
	if(format != bitmap.format)
	{
		LOG_WARNING("BitmapInsert. Форматы изображений не соответствуют.");
		return iRect;
	}
	
	gm::Rectangle srcRect(pos, bitmap.size);
	gm::Rectangle recRect(gm::Point(), size);
	iRect = recRect.Intersect(srcRect);

	for(int i = 0; i < iRect.size.height; i++)
		for(int j = 0; j < iRect.size.width; j++)
		{
			for(unsigned int k = 0; k < stride; k++)
			{
				data[((i + iRect.pos.y) * size.width + j + iRect.pos.x) * stride + k] = 
					bitmap.data[(i * bitmap.size.width + j) * stride + k];
			}
		}

	return iRect;
}

gm::Rectangle Bitmap1::Insert( const Bitmap1 &bitmap, const gm::Rectangle &rect, const gm::Point &pos )
{
	gm::Rectangle iRect;
	if(format != bitmap.format)
	{
		LOG_WARNING("BitmapInsert. Форматы изображений не соответствуют.");
		return iRect;
	}

	// Узнаем область, которую нужно вставить
	gm::Rectangle bitmapRect(gm::Rectangle(gm::Point(), bitmap.size).Intersect(rect));
	// Узнаем облать, куда нужно вставить
	gm::Rectangle srcRect(pos, bitmapRect.size);
	gm::Rectangle recRect(gm::Point(), size);
	iRect = recRect.Intersect(srcRect);

	for(int i = 0; i < iRect.size.height; i++)
		for(int j = 0; j < iRect.size.width; j++)
		{
			for(unsigned int k = 0; k < stride; k++)
			{
				data[((i + iRect.pos.y) * size.width + j + iRect.pos.x) * stride + k] = 
					bitmap.data[((i + bitmapRect.pos.y) * bitmap.size.width + j + bitmapRect.pos.x) * stride + k];
			}
		}

	return iRect;
}

void Bitmap1::Inflate( const gm::Size &_size, const gm::Color &color /*= gm::Color()*/ )
{
	Resize(size + _size, color);
}

void Bitmap1::Resize( const gm::Size &_size, const gm::Color &color /*= gm::Color()*/ )
{
	gm::Size oldSize = size;
	size = _size;

	glm::uint8 *oldData = data;
	data = new glm::uint8[size.width * size.height * stride];

	for(int i = 0; i < size.height; i++)
		for(int j = 0; j < size.width; j++)
		{
			if(i < oldSize.height && j < oldSize.width)
			{
				for(unsigned int k = 0; k < stride; k++)
				{
					data[(i * size.width + j) * stride + k] = oldData[(i * oldSize.width + j) * stride + k];
				}
			}
			else
			{
				SetPixel(gm::Point(j, i), color);
			}
		}
}

Bitmap1::PixelFormat Bitmap1::GetFormat()
{
	return format;
}

glm::uint8 * Bitmap1::GetData()
{
	return data;
}


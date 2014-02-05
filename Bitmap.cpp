#include "Bitmap.h"
#include "Logger.h"

#include <cstdio>
#include <png.h>


static unsigned int GetChannelCount(unsigned int format)
{
	switch (format)
	{
	case Bitmap::FORMAT_LUMINANCE:
	case Bitmap::FORMAT_ALPHA:
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

static bool IsAvailableAlpha(unsigned int format)
{
	switch (format)
	{
	case Bitmap::FORMAT_LUMINANCE_ALPHA:
	case Bitmap::FORMAT_RGBA:
	case Bitmap::FORMAT_ALPHA:
		return true;

	}

	return false;
}

static bool LoadBMP(Bitmap &bitmap, FILE *file)
{

	png_struct* png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	if (!png)
	{
		LOG(LOG_WARNING, "Libng. ���������� ������� png ���������.");
		return false;
	}
	
	png_info* info = png_create_info_struct(png);
	
	if (!info)
	{
		png_destroy_read_struct(&png, NULL, NULL);
		LOG(LOG_WARNING, "Libng. ���������� ������� png info ���������.");
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
		LOG(LOG_WARNING, "Libng. ���������� ������� png ���������.");
		return false;
	}

	png_info* info = png_create_info_struct(png);

	if (!info)
	{
		png_destroy_write_struct(&png, NULL);
		LOG(LOG_WARNING, "Libng. ���������� ������� png info ���������.");
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
			LOG(LOG_WARNING, "Libng. ���������������� ��� �����.");
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
		LOG(LOG_WARNING, "Bitmap. ���������� ������� ���� " + fileName + ".");
		return false;
	}

	if(LoadBMP(*this, file))
	{
		fclose(file);
		return true;
	}

	fclose(file);
	LOG(LOG_WARNING, "Bitmap. ���� " + fileName + " �� ��������.");
	return false;
}

bool Bitmap::Save( std::string fileName )
{
	FILE *file = fopen(fileName.c_str(), "wb");
	if (!file) 
	{
		LOG(LOG_WARNING, "Bitmap. ���������� ������� ���� " + fileName + ".");
		return false;
	}

	if(SaveBMP(*this, file))
	{
		fclose(file);
		return true;
	}

	fclose(file);
	LOG(LOG_WARNING, "Bitmap. ���� " + fileName + " �� ��������.");
	return false;
}

static byte *FormatLuminanceToAny(unsigned int formatOld, unsigned int formatNew, unsigned int width, unsigned int height, byte *data)
{
	unsigned int channelCount = GetChannelCount(formatNew);

	byte *dataNew = new byte[width * height * channelCount];

	unsigned int channelAlpha = (IsAvailableAlpha(formatNew)) ? 1 : 0;

	bool isAlphaOld = IsAvailableAlpha(formatOld);

	for(unsigned int i = 0; i < height * width; i++)
	{
		for (unsigned int k = 0; k < channelCount - channelAlpha; k++)
		{
			dataNew[i * channelCount + k] = (isAlphaOld) ? data[i * 2] : data[i];
		}
		if(channelAlpha > 0)
			dataNew[i * channelCount + channelCount - 1] = (isAlphaOld) ? data[i * 2 + 1] : 255;
	}

	return dataNew;
}

static byte *FormatRGBToAny(unsigned int formatOld, unsigned int formatNew, unsigned int width, unsigned int height, byte *data)
{
	unsigned int channelCountNew = GetChannelCount(formatNew);
	unsigned int channelAlphaNew = (IsAvailableAlpha(formatNew)) ? 1 : 0;

	bool isAlphaOld = IsAvailableAlpha(formatOld);
	unsigned int offsetOld = (isAlphaOld) ? 4 : 3;


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
			dataNew[i * channelCountNew + channelCountNew - 1] = (isAlphaOld) ? data[i * offsetOld + offsetOld - 1] : 255;
	}

	return dataNew;
}



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

#pragma warning (pop)
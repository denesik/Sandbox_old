#include "Bitmap.h"
#include "Logger.h"

#include <cstdio>
#include <png.h>


static unsigned int get_channel_count(unsigned int format)
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

static bool LoadBMP(Bitmap &bitmap, FILE *file)
{
	png_struct* png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	if (!png)
	{
		LOG(LOG_WARNING, "Libng. Невозможно создать png структуру.");
		return false;
	}
	
	png_info* info = png_create_info_struct(png);
	
	if (!info)
	{
		png_destroy_read_struct(&png, NULL, NULL);
		LOG(LOG_WARNING, "Libng. Невозможно создать png info структуру.");
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
			
			// RGB(A) to BGR(A)
			/*case PNG_COLOR_TYPE_RGB:
			 case PNG_COLOR_TYPE_RGB_ALPHA:
			 {
			 png_set_bgr(png);
			 break;
			 }*/
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
		LOG(LOG_WARNING, "Libng. Невозможно создать png структуру.");
		return false;
	}

	png_info* info = png_create_info_struct(png);

	if (!info)
	{
		png_destroy_write_struct(&png, NULL);
		LOG(LOG_WARNING, "Libng. Невозможно создать png info структуру.");
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
	unsigned int channel_count = get_channel_count(format);
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
			LOG(LOG_WARNING, "Libng. неподдерживаемый тип цвета.");
			longjmp(png_jmpbuf(png), 1);
			break;
		}
	}

	png_set_IHDR(png, info, width, height, 8, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
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
}

Bitmap::~Bitmap(void)
{
	if(data)
	{
		delete[] data;
		data = nullptr;
	}
}

void Bitmap::Change(unsigned int format_, unsigned int width_, unsigned int height_, unsigned char *data_)
{
	data = data_;
	width = width_;
	height = height_;
	format = format_;
}

#pragma warning(push)
#pragma warning (disable: 4996)

bool Bitmap::load( std::string fileName )
{
	//FILE *file = fopen(fileName.c_str(), "rb");
	FILE *file = fopen("img.png", "rb");
	if (file == NULL) 
	{
		LOG(LOG_WARNING, "Bitmap. Невозможно открыть файл " + fileName + ".");
		return false;
	}

	if(LoadBMP(*this, file))
	{
		fclose(file);
		return true;
	}

	fclose(file);
	LOG(LOG_WARNING, "Bitmap. Файл " + fileName + " не загружен.");
	return false;
}

bool Bitmap::save( std::string fileName )
{
	FILE *file = fopen(fileName.c_str(), "wb");
	if (!file) 
	{
		LOG(LOG_WARNING, "Bitmap. Невозможно открыть файл " + fileName + ".");
		return false;
	}

	if(SaveBMP(*this, file))
	{
		fclose(file);
		return true;
	}

	fclose(file);
	LOG(LOG_WARNING, "Bitmap. Файл " + fileName + " не загружен.");
	return false;
}

#pragma warning (pop)
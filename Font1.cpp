#include "Font1.h"
#include "Logger.h"
#include <vector>
#include <utf8.h>

Font1::Font1(std::string _fileName, std::string _fontName, unsigned int _size, Atlas *atlas)
{
	face = nullptr;
	library = nullptr;
	fileName = _fileName;
	fontName = _fontName;
	size = _size;
	glyphAtlas = atlas;

	if(!atlas)
	{
		LOG_WARNING("Шрифт %s не инициализирован. Отсутствует атлас.");
		return;
	}

	if (FT_Init_FreeType( &library )) 
	{
		LOG_WARNING("Шрифт %s не инициализирован.", fileName.c_str());
		return;
	}


	if (FT_New_Face( library, fileName.c_str(), 0, &face )) 
	{
		LOG_WARNING("Шрифт %s не инициализирован.", fileName.c_str());
		return;
	}
	FT_Set_Char_Size( face, size << 6, size << 6, 96, 96);

	if(!GenerateEmptyGlyph())
	{
		LOG_WARNING("Шрифт %s. EmptyGlyph не создан.", fontName.c_str());
		return;
	}

}


Font1::~Font1(void)
{
	if(face)
	{
		FT_Done_Face(face);
		face = nullptr;
	}

	if(library)
	{
		FT_Done_FreeType(library);
		library = nullptr;
	}
}

bool Font1::CreateGlyph( std::string utf8glyph )
{
	if(!(library && face && glyphAtlas))
	{
		return false;
	}

	unsigned int glyphNumber;
	std::vector<int> glyphsUTF32;
	utf8::utf8to32(utf8glyph.begin(), utf8glyph.end(), std::back_inserter(glyphsUTF32));
	if(glyphsUTF32.size() <= 0)
	{
		return false;
	}

	glyphNumber = glyphsUTF32[0];
	
	if(FT_Load_Glyph( face, FT_Get_Char_Index( face, glyphNumber ), FT_LOAD_DEFAULT ))
	{
		// Невозможно загрузить глиф.
		return false;
	}

	FT_Glyph glyph;
	if(FT_Get_Glyph( face->glyph, &glyph ))
	{
		// Невозможно загрузить глиф.
		return false;
	}

	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	FT_Bitmap &bitmap=bitmap_glyph->bitmap;

	Bitmap1 img(Bitmap1::FORMAT_RGBA, gm::Size(bitmap.width, bitmap.rows));
	unsigned int channelCount = GetChannelCount(Bitmap1::FORMAT_RGBA);
	glm::uint8 *imgData = img.GetData();

	for(int j = 0; j < bitmap.rows; j++) 
	{
		for(int i = 0; i < bitmap.width; i++)
		{
			imgData[( bitmap.width * j + i ) * channelCount + channelCount - 1] 
			= bitmap.buffer[bitmap.width * j + i];
		}
	}

	std::string glyphName = fontName + "_" + utf8glyph;

	FontGlyphTexture fontGlyphTexture;
	if(!glyphAtlas->Insert(img, glyphName))
	{
		return false;
	}

	fontGlyphTexture.rect = glyphAtlas->GetImagePos(glyphName);

	fontGlyphTexture.fontGlyph.width = bitmap.width;
	fontGlyphTexture.fontGlyph.height = bitmap.rows;
	fontGlyphTexture.fontGlyph.offsetDown = bitmap_glyph->top - bitmap.rows;

	glyphsTextureMap[glyphNumber] = fontGlyphTexture;

	return true;
}

void Font1::Create()
{
	gm::Size atlasSize = glyphAtlas->GetSize();

	for(auto i = glyphsTextureMap.begin(); i != glyphsTextureMap.end(); i++)
	{
		FontGlyphTexture &fgt = (*i).second;
		fgt.fontGlyph.u1 = float(atlasSize.width) / float(fgt.rect.Left());
		fgt.fontGlyph.v1 = float(atlasSize.height) / float(fgt.rect.Bottom());
		fgt.fontGlyph.u2 = float(atlasSize.width) / float(fgt.rect.Right());
		fgt.fontGlyph.v2 = float(atlasSize.height) / float(fgt.rect.Top());
	}

}

bool Font1::GenerateEmptyGlyph()
{

	Bitmap1 img(Bitmap1::FORMAT_RGBA, gm::Size(size, size));
	unsigned int channelCount = GetChannelCount(Bitmap::FORMAT_RGBA);

	byte *glyphImageData = img.GetData();

	unsigned int w = img.GetWidth();
	unsigned int h = img.GetHeight();

	for(unsigned int i = 0; i < w; i++)
	{
		glyphImageData[i * channelCount + channelCount - 1] = 255;
		glyphImageData[(h - 1) * w * channelCount + i * channelCount + channelCount - 1] = 255;
	}
	for(unsigned int j = 0; j < h; j++)
	{
		glyphImageData[j * w * channelCount + channelCount - 1] = 255;
		glyphImageData[j * w * channelCount + (w - 1) * channelCount + channelCount - 1] = 255;
	}

	std::string glyphName = fontName + "_" + "null";

	if(!glyphAtlas->Insert(img, glyphName))
	{
		return false;
	}

	emptyGlyph.rect = glyphAtlas->GetImagePos(glyphName);

	emptyGlyph.fontGlyph.width = size;
	emptyGlyph.fontGlyph.height = size;
	emptyGlyph.fontGlyph.offsetDown = 0;

	return true;
}

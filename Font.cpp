#include "Font.h"
#include "Logger.h"

#include <writer.h>
#include <reader.h>
#include <fstream>
#include <utf8.h>
#include <math.h>
#include <algorithm>
#include "Render.h"
#include "Rectangle.h"

Font *Font::instance = nullptr;

Font::Font()
{
	library = nullptr;
	face = nullptr;

}


Font::~Font()
{
	if(library)
	{
		FT_Done_FreeType(library);
		library = nullptr;
	}
	instance = nullptr;
}

bool Font::Init()
{
	if(!instance)
	{
		instance = new Font();
		if (FT_Init_FreeType( &instance->library )) 
		{
			LOG_ERROR("FreeType не инициализирован.");
			return false;
		}
		// Создаем глиф с кодом 0.
		if(!instance->GenerateEmptyGlyph())
		{
			LOG_ERROR("Глиф с кодом 0 не создан.");
			return false;
		}
	}
	else
	{
		LOG_WARNING("Повторная инициализация. Freetype уже был инициализирован.");
	}

	return true;
}

bool Font::Create( std::string configFileName )
{
	bool processing = true;
	if(!CreateFromConfig(configFileName))
	{
		processing = false;
	}

	if(!GenerateOpenglGlyphs())
	{
		processing = false;
	}

	return processing;
}

void Font::Remove()
{
	glyphsTextureMap.clear();
	// удалить glyphsTextureMap[0].texture
}

bool Font::GenerateEmptyGlyph()
{
	unsigned int key = 0;
	GlyphBitmap glyphBitmap;
	glyphBitmap.bitmap = new Bitmap();
	glyphBitmap.bitmap->Generate(Bitmap::FORMAT_RGBA, 6, 8, 0x00000000);
	unsigned int channelCount = GetChannelCount(Bitmap::FORMAT_RGBA);

	byte *glyphImageData = glyphBitmap.bitmap->GetData();

	unsigned int w = glyphBitmap.bitmap->GetWidth();
	unsigned int h = glyphBitmap.bitmap->GetHeight();

	
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
	glyphBitmap.offsetDown = 0;

	glyphBitmap.key = key;
	glyphsBitmapList.push_back(glyphBitmap);

	return true;
}


bool Font::CreateFromConfig( std::string configFileName )
{
	std::ifstream configFile(configFileName);

	if (!configFile.is_open()) 
	{
		LOG_WARNING("Шрифты. Невозможно открыть конфигурационный файл шрифтов %s.", configFileName);
		return false;
	}


	Json::Value root;
	Json::Reader reader;

	bool parsingSuccessful = reader.parse( configFile, root );
	if ( !parsingSuccessful )
	{
		LOG_WARNING("Шрифты. Ошибка в структуре конфигурационного файла %s. %s", configFileName, reader.getFormatedErrorMessages());
		configFile.close();
		return false;
	}

	const Json::Value fontList = root["FontList"];
	for ( unsigned int i = 0; i < fontList.size(); i++ )
	{
		std::string fontFileName = fontList[i].get("FileName", "").asString();
		unsigned int fontSize = fontList[i].get("Size", 14).asUInt();
		const Json::Value glyphs = fontList[i]["Glyphs"];

		if (FT_New_Face( library, fontFileName.c_str(), 0, &face )) 
		{
			LOG_WARNING("Файл шрифтов %s не загружен.", fontFileName);
			configFile.close();
			return false;
		}
		// По некоторым причинам FreeType измеряет размер шрифта в терминах 1/64 пикселя.
		// Таким образом, для того чтобы сделать шрифт выстой size пикселей, мы запрашиваем размер size*64.
		// (size << 6 тоже самое что и size*64)
		FT_Set_Char_Size( face, fontSize << 6, fontSize << 6, 96, 96);

		// Создаем глифы для заданных наборов символов.
		for ( unsigned int j = 0; j < glyphs.size(); j++ )
		{
			std::string glyphList = glyphs[j].asString();
			GenerateGlyphsList(glyphList);
		}

		if(face)
		{
			FT_Done_Face(face);
			face = nullptr;
		}
	}

	configFile.close();

	return true;
}

Font* Font::GetInstance()
{

	if(!instance)
	{
		LOG_FATAL("Попытка обратиться к несуществеющему классу шрифтов. Инициализируйте класс шрифтов.");
	}

	return instance;
}

bool Font::GenerateGlyphsList( std::string glyphList )
{
	GlyphBitmap glyphBitmap;

	std::vector<int> glyphsUTF32;
	utf8::utf8to32(glyphList.c_str(), glyphList.c_str() + glyphList.length(), std::back_inserter(glyphsUTF32));

	for(unsigned int i = 0; i < glyphsUTF32.size(); i++)
	{	
		// Проверяем есть ли такой символ в нашем списке.
		int key = glyphsUTF32[i];
		if( std::find_if( glyphsBitmapList.begin(), glyphsBitmapList.end(), 
			[&key](GlyphBitmap n) { return n.key == key; } 
		) != glyphsBitmapList.end() )
		{
			// Если уже есть то добавлять не нужно.
			continue;
		}

		glyphBitmap.bitmap = new Bitmap();
		if( GenerateGlyph(key, glyphBitmap) )
		{
			//glyphsBitmapList[glyphsUTF32[i]] = glyphBitmap;
			glyphBitmap.key = key;
			glyphsBitmapList.push_back(glyphBitmap);
		}
		else
		{
			LOG_WARNING("Невозможно загрузить глиф %i", key);
			glyphBitmap.bitmap->Free();
			delete glyphBitmap.bitmap;
			glyphBitmap.bitmap = nullptr;
		}

	}

	return true;
}

bool Font::GenerateGlyph( unsigned int glyphNumber, GlyphBitmap &glyphBitmap)
{

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


	glyphBitmap.bitmap->Generate(Bitmap::FORMAT_RGBA, bitmap.width, bitmap.rows, 0x00000000);
	unsigned int channelCount = GetChannelCount(Bitmap::FORMAT_RGBA);

	byte *glyphImageData = glyphBitmap.bitmap->GetData();

	for(int j = 0; j < bitmap.rows; j++) 
	{
		for(int i=0; i < bitmap.width; i++)
		{
 			glyphImageData[( bitmap.width * j + i ) * channelCount + channelCount - 1] 
 				= bitmap.buffer[bitmap.width * ( bitmap.rows - j - 1) + i];
		}
	}
	
	glyphBitmap.offsetDown = bitmap_glyph->top - bitmap.rows;

	return true;
}

bool Font::GenerateOpenglGlyphs()
{
	unsigned int area = 0;
	Bitmap *bitmap;

	// Вычисляем площадь всех символов
	for (auto i = glyphsBitmapList.begin(); i != glyphsBitmapList.end(); i++)
	{
		bitmap = (*i).bitmap;
		area += bitmap->GetHeight() * bitmap->GetWidth();
	}

	unsigned int sideAtlas = (unsigned int)(sqrt( float(area) ));
	sideAtlas = next_p2( sideAtlas );

	float diff = float(sideAtlas * sideAtlas) / float(area);
	if( diff < 1.8f )
		sideAtlas <<= 1;

	if(!glyphAtlas.Create(Bitmap::FORMAT_RGBA, sideAtlas, sideAtlas))
	{
		LOG_ERROR("Шрифты. Текстурный атлас не создан.");
		return false;
	}

	// Сортируем по убыванию площадей
	glyphsBitmapList.sort();

	iRect rect;
	Bitmap *atlasBitmap = glyphAtlas.GetAtlas();

	for (auto i = glyphsBitmapList.begin(); i != glyphsBitmapList.end(); i++)
	{
		if( !glyphAtlas.InsertImage( (*i).bitmap, rect ) ) 
		{
			LOG_WARNING("Не удалось загрузить глиф %i в атлас.", (*i).key);
			delete (*i).bitmap;
			(*i).bitmap = nullptr;
			continue;
		}

		FontTexture fontTexture;

		fontTexture.width = (*i).bitmap->GetWidth();
		fontTexture.height = (*i).bitmap->GetHeight();
		fontTexture.offsetDown = (*i).offsetDown;

		GenerateTextCoord(atlasBitmap, &rect, fontTexture.texture);

		glyphsTextureMap[(*i).key] = fontTexture;

		delete (*i).bitmap;
		(*i).bitmap = nullptr;
	}

	// CreateAtlasOpenglTexture
	unsigned int ogltexture = GenerateOpenglBitmap(*atlasBitmap, false, false);
	for (auto i = glyphsTextureMap.begin(); i != glyphsTextureMap.end(); i++)
	{
		(*i).second.texture.textureId = ogltexture;
	}

	glyphsBitmapList.clear();
	glyphAtlas.GetAtlas()->Save("atlas.png");

	glyphAtlas.Remove();
	return true;
}

FontTexture Font::GetGlyphTexture( unsigned int utf32glyph )
{
	if(glyphsTextureMap.count(utf32glyph) == 0)
	{
		return glyphsTextureMap[0];
	}
	return glyphsTextureMap[utf32glyph];
}


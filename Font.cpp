#include "Font.h"
#include "Logger.h"

#include<json/writer.h>
#include<json/reader.h>
#include <fstream>
#include <utf8.h>
#include <math.h>
#include <algorithm>
#include "Render.h"
#include "Rectangle.h"

Font::Font( std::string configFileName )
{
	library = nullptr;
	face = nullptr;
	Init();

	LoadConfig( configFileName );

	Finalize();

}


Font::~Font(void)
{
	
}

bool Font::Init()
{
	if (FT_Init_FreeType( &library )) 
	{
		//LOG(LOG_ERROR, "FreeType не инициализирвоан.");
		return false;
	}
	return true;
}

void Font::Finalize()
{
	// delete ogl image atlas

	if(library)
	{
		FT_Done_FreeType(library);
		library = nullptr;
	}
}

bool Font::GenerateGlyph( unsigned int glyphNumber, GlyphBitmap &glyphBitmap)
{

	if(FT_Load_Glyph( face, FT_Get_Char_Index( face, glyphNumber ), FT_LOAD_DEFAULT ))
	{
		//LOG(LOG_WARNING, "FreeType. Невозможно загрузить глиф.");
		return false;
	}

	FT_Glyph glyph;
	if(FT_Get_Glyph( face->glyph, &glyph ))
	{
		//LOG(LOG_WARNING, "FreeType. Невозможно загрузить глиф.");
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

bool Font::LoadConfig( std::string configFileName )
{
	std::ifstream configFile(configFileName);

	if (!configFile.is_open()) 
	{
		//LOG(LOG_ERROR, "Шрифты. Невозможно открыть конфигурационный файл шрифтов " + configFileName + ".");
		return 0;
	}


	Json::Value root;
	Json::Reader reader;

	bool parsingSuccessful = reader.parse( configFile, root );
	if ( !parsingSuccessful )
	{
		//LOG(LOG_ERROR, "Шрифты. Ошибка в структуре конфигурационного файла " + configFileName + ". " + reader.getFormatedErrorMessages());
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
			//LOG(LOG_WARNING, "FreeType. Файл шрифтов " + fontFileName + " не загружен.");
			return false;
		}
		// По некоторым причинам FreeType измеряет размер шрифта в терминах 1/64 пикселя.
		// Таким образом, для того чтобы сделать шрифт выстой size пикселей, мы запрашиваем размер size*64.
		// (size << 6 тоже самое что и size*64)
		FT_Set_Char_Size( face, fontSize << 6, fontSize << 6, 96, 96);

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

	GenerateOpenglGlyphs();

	configFile.close();
	return true;
}

bool Font::GenerateGlyphsList( std::string glyphList )
{
	GlyphBitmap glyphBitmap;

	std::vector<int> glyphsUTF32;
	utf8::utf8to32(glyphList.c_str(), glyphList.c_str() + glyphList.length(), std::back_inserter(glyphsUTF32));

	for(unsigned int i = 0; i < glyphsUTF32.size(); i++)
	{
		int key = glyphsUTF32[i];
		if( std::find_if( glyphsBitmapList.begin(), glyphsBitmapList.end(), 
			[&key](GlyphBitmap n) { return n.key == key; } 
			) != glyphsBitmapList.end() )
		{
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
			glyphBitmap.bitmap->Free();
			delete glyphBitmap.bitmap;
			glyphBitmap.bitmap = nullptr;
		}

	}

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

	glyphAtlas.Create(Bitmap::FORMAT_RGBA, sideAtlas, sideAtlas);

	// Сортируем по убыванию площадей
	glyphsBitmapList.sort();

	iRect rect;
	Bitmap *atlasBitmap = glyphAtlas.GetAtlas();

	for (auto i = glyphsBitmapList.begin(); i != glyphsBitmapList.end(); i++)
	{
		if( !glyphAtlas.InsertImage( (*i).bitmap, rect ) ) 
		{
			//LOG(LOG_WARNING, "Font. Символ не загружен.");
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
	unsigned int ogltexture = GenerateOpenglBitmap(*atlasBitmap, false);
	for (auto i = glyphsTextureMap.begin(); i != glyphsTextureMap.end(); i++)
	{
		(*i).second.texture.textureId = ogltexture;
	}

	glyphsBitmapList.clear();
	glyphAtlas.GetAtlas()->Save("atlas.png");

	glyphAtlas.Remove();
	return true;
}


ArrayIndex &Font::Print( float x, float y, std::vector<unsigned int> text, Render *render)
{
	Rectangle geometryRectangle;
	FontTexture fontTexture = glyphsTextureMap[97];

	unsigned int textLenght = text.size();

	float glyphX = x;
	float glyphY = y;
	float stringHeight = 22.0f;

	for(unsigned int i = 0; i < textLenght; i++)
	{
		fontTexture = glyphsTextureMap[text[i]];

		geometryRectangle.SetPos(vec3(glyphX, glyphY + stringHeight - fontTexture.height - fontTexture.offsetDown, -1));
		geometryRectangle.SetSize((float)fontTexture.width, (float)fontTexture.height);
		geometryRectangle.SetTexture(fontTexture.texture);

		glyphX += fontTexture.width;

		ArrayVertex &glyphArrayVertex = geometryRectangle.GetVertexPosition();
		arrayVertex.insert(arrayVertex.end(), glyphArrayVertex.begin(), glyphArrayVertex.end());

		ArrayTextureCoord &glyphArrayTextureCoord = geometryRectangle.GetTextureCoord();
		arrayTextureCoord.insert(arrayTextureCoord.end(), glyphArrayTextureCoord.begin(), glyphArrayTextureCoord.end());

		ArrayIndex &glyphArrayIndex = geometryRectangle.GetVertexIndex();

		unsigned int vertexCount = glyphArrayVertex.size();
		for(unsigned int j = 0; j < glyphArrayIndex.size(); j++)
		{
			glyphArrayIndex[j] += 4 * i;
		}
		arrayIndex.insert(arrayIndex.end(), glyphArrayIndex.begin(), glyphArrayIndex.end());

	}

	render->CreateBufferVertex(arrayVertex);
	render->CreateBufferTextCoord(arrayTextureCoord);
	render->CreateBufferIndex(arrayIndex);

	glBindTexture(GL_TEXTURE_2D, fontTexture.texture.textureId);

	return arrayIndex;

}

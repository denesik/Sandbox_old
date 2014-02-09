#include "Font.h"
#include "Logger.h"

#include<json/writer.h>
#include<json/reader.h>
#include <fstream>
#include <utf8.h>
#include <math.h>
#include <algorithm>

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
		LOG(LOG_ERROR, "FreeType �� ���������������.");
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

bool Font::GenerateGlyph( unsigned int glyphNumber, GlyphBitmap glyphBitmap)
{

	if(FT_Load_Glyph( face, FT_Get_Char_Index( face, glyphNumber ), FT_LOAD_DEFAULT ))
	{
		LOG(LOG_WARNING, "FreeType. ���������� ��������� ����.");
		return false;
	}

	FT_Glyph glyph;
	if(FT_Get_Glyph( face->glyph, &glyph ))
	{
		LOG(LOG_WARNING, "FreeType. ���������� ��������� ����.");
		return false;
	}

	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	FT_Bitmap &bitmap=bitmap_glyph->bitmap;


	glyphBitmap.bitmap->Generate(Bitmap::FORMAT_LUMINANCE_ALPHA, bitmap.width, bitmap.rows, 0x00000000);
	unsigned int channelCount = GetChannelCount(Bitmap::FORMAT_LUMINANCE_ALPHA);

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
		LOG(LOG_ERROR, "������. ���������� ������� ���������������� ���� ������� " + configFileName + ".");
		return 0;
	}

	Json::Value root;
	Json::Reader reader;

	bool parsingSuccessful = reader.parse( configFile, root );
	if ( !parsingSuccessful )
	{
		LOG(LOG_ERROR, "������. ������ � ��������� ����������������� ����� " + configFileName + ". " + reader.getFormatedErrorMessages());
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
			LOG(LOG_WARNING, "FreeType. ���� ������� " + fontFileName + " �� ��������.");
			return false;
		}
		// �� ��������� �������� FreeType �������� ������ ������ � �������� 1/64 �������.
		// ����� �������, ��� ���� ����� ������� ����� ������ size ��������, �� ����������� ������ size*64.
		// (size << 6 ���� ����� ��� � size*64)
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

	// ��������� ������� ���� ��������
	for (auto i = glyphsBitmapList.begin(); i != glyphsBitmapList.end(); i++)
	{
		bitmap = (*i).bitmap;
		area += bitmap->GetHeight() * bitmap->GetWidth();
	}

	unsigned int sideAtlas = (unsigned int)(sqrt( float(area) ));
	sideAtlas = next_p2( sideAtlas );

	float diff = float(sideAtlas * sideAtlas) / float(area);
	if( diff < 1.5f )
		sideAtlas <<= 1;

	glyphAtlas.Create(Bitmap::FORMAT_LUMINANCE_ALPHA, sideAtlas, sideAtlas);

	// ��������� �� �������� ��������
	glyphsBitmapList.sort();

	iRect rect;
	for (auto i = glyphsBitmapList.begin(); i != glyphsBitmapList.end(); i++)
	{
		glyphAtlas.InsertImage( (*i).bitmap, rect );
		
		delete (*i).bitmap;
		(*i).bitmap = nullptr;
	}

	glyphsBitmapList.clear();
	glyphAtlas.GetAtlas()->Save("atlas.png");


	glyphAtlas.Remove();
	return true;
}
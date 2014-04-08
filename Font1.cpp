#include "Font1.h"
#include "Logger.h"
#include <vector>
#include <value.h>
#include <writer.h>
#include <reader.h>


#include <sstream>
template< typename T >
std::string ToString( const T& val )
{
	std::stringstream iss;
	iss << val;
	return iss.str();
}


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

Font1::Font1( std::string configFileName, Atlas *atlas )
{
	face = nullptr;
	library = nullptr;
	fileName = "";
	fontName = "";
	size = 0;
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


	std::ifstream configFile(configFileName);

	if (!configFile.is_open()) 
	{
		LOG_WARNING("Файл конфигурации шрифтов %s не загружен.", configFileName.c_str());
		return ;
	}

	Json::Value root;
	Json::Reader reader;

	bool parsingSuccessful = reader.parse( configFile, root );
	if ( !parsingSuccessful )
	{
		LOG_WARNING("Загрузка шрифтов. Ошибка в структуре конфигурационного файла %s. %s", configFileName.c_str(), reader.getFormatedErrorMessages());
		configFile.close();
		return ;
	}

	fontName = root["FontName"].asString();
	fileName = root["FileName"].asString();
	size = root["Size"].asUInt();

	if (FT_New_Face( library, fileName.c_str(), 0, &face )) 
	{
		LOG_WARNING("Шрифт %s не инициализирован.", fileName.c_str());
		return;
	}
	FT_Set_Char_Size( face, size << 6, size << 6, 96, 96);

	emptyGlyph.rect.UnSerialize(root["EmptyGlyph"]["Rect"]);
	emptyGlyph.fontGlyph.u1 = (float)root["EmptyGlyph"]["FontGlyph"]["u1"].asDouble();
	emptyGlyph.fontGlyph.v1 = (float)root["EmptyGlyph"]["FontGlyph"]["v1"].asDouble();
	emptyGlyph.fontGlyph.u2 = (float)root["EmptyGlyph"]["FontGlyph"]["u2"].asDouble();
	emptyGlyph.fontGlyph.v2 = (float)root["EmptyGlyph"]["FontGlyph"]["v2"].asDouble();

	emptyGlyph.fontGlyph.width = root["EmptyGlyph"]["FontGlyph"]["width"].asUInt();
	emptyGlyph.fontGlyph.height = root["EmptyGlyph"]["FontGlyph"]["height"].asUInt();
	emptyGlyph.fontGlyph.offsetDown = root["EmptyGlyph"]["FontGlyph"]["offsetDown"].asInt();

	if(emptyGlyph.rect.IsAreaNull())
	{
		if(!GenerateEmptyGlyph())
		{
			LOG_WARNING("Шрифт %s. EmptyGlyph не создан.", fontName.c_str());
			return;
		}
	}

	const Json::Value igtm = root["GlyphsTextureMap"];

	for(unsigned int i = 0; i < igtm.size(); i++)
	{
		FontGlyphTexture fgt;
		fgt.rect.UnSerialize(igtm[i]["Rect"]);
		emptyGlyph.fontGlyph.u1 = (float)igtm[i]["FontGlyph"]["u1"].asDouble();
		emptyGlyph.fontGlyph.v1 = (float)igtm[i]["FontGlyph"]["v1"].asDouble();
		emptyGlyph.fontGlyph.u2 = (float)igtm[i]["FontGlyph"]["u2"].asDouble();
		emptyGlyph.fontGlyph.v2 = (float)igtm[i]["FontGlyph"]["v2"].asDouble();

		emptyGlyph.fontGlyph.width = igtm[i]["FontGlyph"]["width"].asUInt();
		emptyGlyph.fontGlyph.height = igtm[i]["FontGlyph"]["height"].asUInt();
		emptyGlyph.fontGlyph.offsetDown = igtm[i]["FontGlyph"]["offsetDown"].asInt();

		glyphsTextureMap[igtm[i]["GlyphNumber"].asUInt()] = fgt;
	}

	configFile.close();
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


bool Font1::CreateGlyph( unsigned int utf32glyph )
{
	if(!(library && face && glyphAtlas))
	{
		return false;
	}

	if(FT_Load_Glyph( face, FT_Get_Char_Index( face, utf32glyph ), FT_LOAD_DEFAULT ))
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

	std::string glyphName = fontName + "_" + ToString(utf32glyph);

	FontGlyphTexture fontGlyphTexture;
	if(!glyphAtlas->Insert(img, glyphName))
	{
		return false;
	}

	fontGlyphTexture.rect = glyphAtlas->GetImagePos(glyphName);

	fontGlyphTexture.fontGlyph.width = bitmap.width;
	fontGlyphTexture.fontGlyph.height = bitmap.rows;
	fontGlyphTexture.fontGlyph.offsetDown = bitmap_glyph->top - bitmap.rows;

	glyphsTextureMap[utf32glyph] = fontGlyphTexture;

	return true;
}

bool Font1::Create()
{
	if(!(library && face && glyphAtlas))
	{
		return false;
	}

	gm::Size atlasSize = glyphAtlas->GetSize();

	if(atlasSize.IsEmpty())
	{
		return false;
	}

	if(emptyGlyph.rect.IsAreaNull())
	{
		return false;
	}

	emptyGlyph.fontGlyph.u1 = float(emptyGlyph.rect.Left()) / float(atlasSize.width);
	emptyGlyph.fontGlyph.v1 = float(emptyGlyph.rect.Bottom()) / float(atlasSize.height);
	emptyGlyph.fontGlyph.u2 = float(emptyGlyph.rect.Right()) / float(atlasSize.width);
	emptyGlyph.fontGlyph.v2 = float(emptyGlyph.rect.Top()) / float(atlasSize.height);

	for(auto i = glyphsTextureMap.begin(); i != glyphsTextureMap.end(); i++)
	{
		FontGlyphTexture &fgt = (*i).second;
		fgt.fontGlyph.u1 = float(fgt.rect.Left()) / float(atlasSize.width);
		fgt.fontGlyph.v1 = float(fgt.rect.Bottom()) / float(atlasSize.height);
		fgt.fontGlyph.u2 = float(fgt.rect.Right()) / float(atlasSize.width);
		fgt.fontGlyph.v2 = float(fgt.rect.Top()) / float(atlasSize.height);
	}

	return true;
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

	if(glyphAtlas->GetImagePos(glyphName).IsEmpty())
	{
		if(!glyphAtlas->Insert(img, glyphName))
		{
			return false;
		}
	}

	emptyGlyph.rect = glyphAtlas->GetImagePos(glyphName);

	emptyGlyph.fontGlyph.width = size;
	emptyGlyph.fontGlyph.height = size;
	emptyGlyph.fontGlyph.offsetDown = 0;

	return true;
}

const FontGlyph & Font1::GetGlyph( unsigned int utf32glyph ) const
{
	auto i = glyphsTextureMap.find(utf32glyph);
	if(i == glyphsTextureMap.end())
	{
		return emptyGlyph.fontGlyph;
	}

	return (*i).second.fontGlyph;
}

bool Font1::Save( std::string dir /*= ""*/ ) const
{
	if(!(library && face && glyphAtlas))
	{
		LOG_WARNING("Не удалось сохранить шрифт %s.", fontName.c_str());
		return false;
	}

	gm::Size atlasSize = glyphAtlas->GetSize();

	if(atlasSize.IsEmpty() || emptyGlyph.rect.IsAreaNull())
	{
		LOG_WARNING("Не удалось сохранить шрифт %s.", fontName.c_str());
		return false;
	}


	std::string fileDir = dir;
	if(dir.size() > 0)
		fileDir += "/";

	std::ofstream configFile(fileDir + fontName + ".json");

	Json::Value root;

	root["FontName"] = fontName;
	root["FileName"] = fileName;
	root["Size"] = size;

	root["EmptyGlyph"]["Rect"] = emptyGlyph.rect.Serialize();
	root["EmptyGlyph"]["FontGlyph"]["u1"] = emptyGlyph.fontGlyph.u1;
	root["EmptyGlyph"]["FontGlyph"]["v1"] = emptyGlyph.fontGlyph.v1;
	root["EmptyGlyph"]["FontGlyph"]["u2"] = emptyGlyph.fontGlyph.u2;
	root["EmptyGlyph"]["FontGlyph"]["v2"] = emptyGlyph.fontGlyph.v2;

	root["EmptyGlyph"]["FontGlyph"]["width"] = emptyGlyph.fontGlyph.width;
	root["EmptyGlyph"]["FontGlyph"]["height"] = emptyGlyph.fontGlyph.height;
	root["EmptyGlyph"]["FontGlyph"]["offsetDown"] = emptyGlyph.fontGlyph.offsetDown;

	unsigned int igtm = 0;
	for(auto i = glyphsTextureMap.begin(); i != glyphsTextureMap.end(); i++)
	{
		const FontGlyphTexture &fgt = (*i).second;
		root["GlyphsTextureMap"][igtm]["GlyphNumber"] = (*i).first;
		root["GlyphsTextureMap"][igtm]["Rect"] = fgt.rect.Serialize();
		root["GlyphsTextureMap"][igtm]["FontGlyph"]["u1"] = fgt.fontGlyph.u1;
		root["GlyphsTextureMap"][igtm]["FontGlyph"]["v1"] = fgt.fontGlyph.v1;
		root["GlyphsTextureMap"][igtm]["FontGlyph"]["u2"] = fgt.fontGlyph.u2;
		root["GlyphsTextureMap"][igtm]["FontGlyph"]["v2"] = fgt.fontGlyph.v2;

		root["GlyphsTextureMap"][igtm]["FontGlyph"]["width"] = fgt.fontGlyph.width;
		root["GlyphsTextureMap"][igtm]["FontGlyph"]["height"] = fgt.fontGlyph.height;
		root["GlyphsTextureMap"][igtm]["FontGlyph"]["offsetDown"] = fgt.fontGlyph.offsetDown;

		igtm++;
	}

	configFile << root;
	configFile.close();

	return true;
}

bool Font1::FindGlyph( unsigned int utf32glyph ) const
{
	auto i = glyphsTextureMap.find(utf32glyph);
	if(i == glyphsTextureMap.end())
	{
		return false;
	}

	return true;
}

std::string Font1::GetName() const
{
	return fontName;
}

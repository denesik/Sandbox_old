#include "ResourceManager.h"
#include <value.h>
#include <writer.h>
#include <reader.h>
#include <iostream>
#include "Logger.h"
#include <utf8.h>


ResourceManager::ResourceManager(void)
{
	fontsConfig = "fonts/fonts.json";

	fontAtlas = nullptr;
}


ResourceManager::~ResourceManager(void)
{
	if(fontAtlas)
	{
		delete fontAtlas;
		fontAtlas = nullptr;
	}
}


Font * ResourceManager::LoadFont( std::string type )
{
	if(!fontAtlas)
	{
		fontAtlas = new ImageAtlas("fontAtlas", Bitmap::FORMAT_RGBA, 1, gm::Size(2048, 2048), gm::Size(64, 64));
	}

	std::ifstream configFile(fontsConfig);

	if (!configFile.is_open()) 
	{
		LOG_WARNING("Файл конфигурации шрифтов %s не загружен.", fontsConfig.c_str());
		return nullptr;
	}

	Json::Value root;
	Json::Reader reader;

	bool parsingSuccessful = reader.parse( configFile, root );
	if ( !parsingSuccessful )
	{
		LOG_WARNING("Загрузка шрифтов. Ошибка в структуре конфигурационного файла %s. %s", fontsConfig.c_str(), reader.getFormatedErrorMessages());
		configFile.close();
		return nullptr;
	}

	std::string fontName =	root[type]["FontName"].asString();
	std::string fileName =	root[type]["FileName"].asString();
	unsigned int size =		root[type]["Size"].asUInt();

	std::vector<std::string> glyphsUTF8;
	const Json::Value glyphsLines = root[type]["Glyphs"];

	for(unsigned int i = 0; i < glyphsLines.size(); i++)
	{
		glyphsUTF8.push_back(glyphsLines[i].asString());
	}

	Font *font = nullptr;


	font = new Font(fileName, fontName, size, fontAtlas);

	// генерируем глифы
	for(unsigned int i = 0; i < glyphsUTF8.size(); i++)
	{
		std::vector<int> utf32result;
		utf8::utf8to32(glyphsUTF8[i].begin(), glyphsUTF8[i].end(), back_inserter(utf32result));

		for(unsigned int j = 0; j < utf32result.size(); j++)
		{
			font->CreateGlyph(utf32result[j]);
		}
	}

	fontsList.push_back(font);

	return font;
}

void ResourceManager::CreateFonts()
{
	for (auto i = fontsList.begin(); i != fontsList.end(); i++)
	{
		(*i)->Create();
	}
	fontAtlas->GetBitmap()->Save("fonts/fonts.png");
}

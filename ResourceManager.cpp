#include "ResourceManager.h"
#include <value.h>
#include <writer.h>
#include <reader.h>
#include <iostream>
#include "Logger.h"


ResourceManager::ResourceManager(void)
{
	fontsConfig = "fonts/fonts.json";
	fontAtlas = "fonts/fontAtlas.json";
}


ResourceManager::~ResourceManager(void)
{
}

Font1 *ResourceManager::LoadDefaultFont()
{
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
		LOG_WARNING("Загрузка шрифтов. Ошибка в структуре конфигурационного файла %s. %s", configFile, reader.getFormatedErrorMessages());
		configFile.close();
		return nullptr;
	}

	std::string fontName = root["Default"].asString();

}

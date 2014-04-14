#ifndef ResourceManager_h__
#define ResourceManager_h__

#include "Font.h"
#include <vector>

class ResourceManager
{
private:
	std::string fontsConfig;

	ImageAtlas *fontAtlas;

	std::vector<Font *> fontsList;

public:
	ResourceManager(void);
	~ResourceManager(void);

	Font *LoadFont(std::string type);

	void CreateFonts();

};


#endif // ResourceManager_h__
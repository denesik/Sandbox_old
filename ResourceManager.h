#ifndef ResourceManager_h__
#define ResourceManager_h__

#include "Font1.h"
#include <vector>

class ResourceManager
{
private:
	std::string fontsConfig;

	Atlas *fontAtlas;

	std::vector<Font1 *> fontsList;

public:
	ResourceManager(void);
	~ResourceManager(void);

	Font1 *LoadFont(std::string type);

	void CreateFonts();

};


#endif // ResourceManager_h__
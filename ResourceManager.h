#ifndef ResourceManager_h__
#define ResourceManager_h__

#include "Font1.h"

class ResourceManager
{
private:
	std::string fontsConfig;
	std::string fontAtlas;

public:
	ResourceManager(void);
	~ResourceManager(void);

	Font1 *LoadDefaultFont();

};


#endif // ResourceManager_h__
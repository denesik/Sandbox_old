#ifndef BlockStatic_h__
#define BlockStatic_h__

#include "Cube.h"
#include <string>

class BlockStatic
{
private:
	std::string textureFileName;

	Cube geometry;
	ImageAtlas atlas;


public:
	BlockStatic(void);
	~BlockStatic(void);

	bool LoadContent();

	Cube &GetGraphicData();
};


#endif // BlockStatic_h__
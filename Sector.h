#ifndef Sector_h__
#define Sector_h__

#include "BlockStatic.h"
#include "BufferArray.h"

class Sector
{
private:
	int map[64][25][64];
	BlockStatic block;

	BufferArray<V3T2> buffer;

public:
	Sector(void);
	~Sector(void);

	void LoadContent();

	void CreateGeometry();

	void Draw();

};


#endif // Sector_h__
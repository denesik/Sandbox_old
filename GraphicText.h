#ifndef GraphicText_h__
#define GraphicText_h__

#include <string>
#include <vector>
#include "Render.h"

class GraphicText
{
private:
	std::vector<uint32_t> utf32text;

	ArrayVertex arrayVertex;
	ArrayTextureCoord arrayTextureCoord;
	ArrayIndex arrayIndex;

	float constraintWidth;
	float constraintHeight;

public:
	GraphicText(void);
	~GraphicText(void);

	void SetText(std::string text);

};


#endif // GraphicText_h__
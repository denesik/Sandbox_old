#ifndef GraphicText_h__
#define GraphicText_h__

#include <string>
#include <vector>
#include "Render.h"

class GraphicText
{
private:
	std::vector<uint32_t> utf32text;

	float constraintWidth;
	float constraintHeight;

public:
	GraphicText(void);
	~GraphicText(void);

	void SetText(std::string text);

};


#endif // GraphicText_h__
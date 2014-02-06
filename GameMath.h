#ifndef GameMath_h__
#define GameMath_h__

#include "glm.hpp"

using namespace glm;

struct iRect
{
	int x;
	int y;
	int w;
	int h;

	iRect()
	{
		x = 0; y = 0; w = 0; h = 0;
	}

	iRect(int x_, int y_, int w_, int h_)
	{
		x = x_;
		y = y_;
		w = w_;
		h = h_;
	}
};

#endif // GameMath_h__
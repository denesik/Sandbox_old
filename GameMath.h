#ifndef GameMath_h__
#define GameMath_h__

#include "glm.hpp"

using namespace glm;

inline int next_p2( int a )
{
	int rval = 1;
	while( rval < a ) rval <<= 1;
	return rval;
}

inline unsigned int next_p2( unsigned int a )
{
	unsigned int rval = 1;
	while( rval < a ) rval <<= 1;
	return rval;
}

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
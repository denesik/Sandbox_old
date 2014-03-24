#ifndef Vertex_h__
#define Vertex_h__

#include "TypeList.h"

enum VertexType
{
	VERTEX_TYPE_VERTEX = 1,
	VERTEX_TYPE_COLOR,
	VERTEX_TYPE_TEXTCOORD,
	VERTEX_TYPE_NORMALE,
};

struct Vertex
{
	enum { Type = VERTEX_TYPE_VERTEX };
	enum { Size = 3 };
	union
	{
		float x, y, z;
		float data[Size];
	};
};

struct TextCoord
{
	enum { Type = VERTEX_TYPE_TEXTCOORD };
	enum { Size = 2 };
	union
	{
		float u, v;
		float data[Size];
	};
};

struct Color
{
	enum { Type = VERTEX_TYPE_COLOR };
	enum { Size = 4 };
	union
	{
		float R, G, B, A;
		float data[Size];
	};
};


typedef TYPELIST_3(Vertex, TextCoord, Color) VTC;
typedef TYPELIST_3(Vertex, Color, TextCoord) VCT;
typedef TYPELIST_2(Vertex, TextCoord) VT;





#endif // Vertex_h__
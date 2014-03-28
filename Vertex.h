#ifndef Vertex_h__
#define Vertex_h__

#include "TypeList.h"


// Узнаем количество элементов в вершине
template <class TList> struct VertexStride;
template <> struct VertexStride<NullType>
{
	enum { value = 0 };
};
template <class Head,class Tail>
struct VertexStride< TypeList< Head, Tail> >
{
	enum { value = Head::Size + VertexStride<Tail>::value };
};


// Узнаем смещение в списке до типа T
template <class TList, class T> struct VertexCount;
template <class T>
struct VertexCount<NullType, T>
{
	// Возникнет ошибка, если типа нет в списке типов
	//enum { value = 0 };
};
template <class T, class Tail>
struct VertexCount<TypeList<T, Tail>, T>
{
	enum { value = 0 };
};
template <class Head, class Tail, class T>
struct VertexCount<TypeList<Head, Tail>, T>
{
	enum { value = Head::Size + VertexCount<Tail, T>::value };
};



template <class TList, class T>
inline T &VertexAccess(float *data, unsigned int number)
{
	return reinterpret_cast<T&>( data[ VertexCount<TList, T>::value + VertexStride<TList>::value * number ] );
};




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
		struct 
		{
			float x, y, z;
		};
		float data[Size];
	};
	Vertex(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
};

struct TextCoord
{
	enum { Type = VERTEX_TYPE_TEXTCOORD };
	enum { Size = 2 };
	union
	{
		struct 
		{
			float u, v;
		};
		float data[Size];
	};
	TextCoord(float _u, float _v)
	{
		u = _u;
		v = _v;
	}
};

struct Color
{
	enum { Type = VERTEX_TYPE_COLOR };
	enum { Size = 4 };
	union
	{
		struct 
		{
			float R, G, B, A;
		};
		float data[Size];
	};
};


typedef TYPELIST_3(Vertex, TextCoord, Color) VTC;
typedef TYPELIST_3(Vertex, Color, TextCoord) VCT;
typedef TYPELIST_2(Vertex, TextCoord) VT;





#endif // Vertex_h__
#ifndef Vertex_h__
#define Vertex_h__


enum VertexType
{
	VERTEX_TYPE_VERTEX,
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

class NullType{};
struct EmptyType{};

template <class T, class U>
struct TypeList
{
	typedef T head;
	typedef U Tail;
};

#define TYPELIST_1(T1) TypeList<T1, NullType>
#define TYPELIST_2(T1, T2) TypeList<T1, TYPELIST_1(T2)>
#define TYPELIST_3(T1, T2, T3) TypeList<T1, TYPELIST_2(T2, T3)>
#define TYPELIST_4(T1, T2, T3, T4) TypeList<T1, TYPELIST_3(T2, T3, T4)>
#define TYPELIST_5(T1, T2, T3, T4, T5) TypeList<T1, TYPELIST_4(T2, T3, T4, T5)>

typedef TYPELIST_3(Vertex, Color, TextCoord) VTC;


#endif // Vertex_h__
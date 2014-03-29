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

namespace gm
{
	struct Size;

	struct Point
	{
		int x;
		int y;

		Point();
		Point(int val);
		Point(int x, int y);
		Point(const Point &p);
		~Point(){};

		// ������� �����
		inline void Offset(const Point &p);
		inline void Offset(int x, int y);

		// ���������� �����
		inline bool Equals(const Point &p) const;
		inline bool Equals(int x, int y) const;

		// ���������� true ���� x � y ����� 0
		inline bool IsEmpty() const;


		inline const Point& operator=(const Point& p);

		// ������� ����� Point �� �������� �������� Size.
		inline const Point operator+(const Size& s) const;

		inline const Point operator-(const Size& s) const;

		inline const bool operator==(const Point& p) const;

		inline const bool operator!=(const Point& p) const;

	};

	struct Size
	{
		int width;
		int height;

		Size();
		Size(const Point &p);
		Size(const Size &s);
		Size(int width, int height);

		// ���������� true ���� w � h ����� 0
		inline bool IsEmpty() const;

		inline void Add(const Size &s);
		inline void Subtract(const Size &s);

		inline bool Equals(const Size &s) const;
		inline bool Equals(int widht, int height) const;


		inline const Size& operator=(const Size& s);

		inline const Size operator+(const Size& s) const;

		inline const Size operator-(const Size& s) const; 

		inline const bool operator==(const Size& s) const;

		inline const bool operator!=(const Size& s) const;

	};

	struct Rectangle
	{
		union
		{
			struct 
			{
				int x, y, w, h;
			};
			struct  
			{
				Point pos;
				Size size;
			};
		};

		Rectangle();
		Rectangle(const Point &p, const Size &s);
		Rectangle(int x, int y, int w, int h);
		Rectangle(const Rectangle &rectangle);

		inline bool IsEmpty() const;

		inline int Bottom() const;
		inline int Top() const;
		inline int Left() const;
		inline int Right() const;

		// ����������, ���������� �� �������� ����� � ��������� Rectangle.
		inline bool Contains(const Point &p) const;
		inline bool Contains(int x, int y) const;
		inline bool Contains(const Rectangle &r) const;

		inline bool Equals(const Rectangle &r) const;

		// ����������� ������ ������ Rectangle �� �������� ��������.
		inline void Inflate(const Size &s);
		inline void Inflate(int w, int h);

		// ���������� ����������� ������� �������������� � ��������������� r.
		inline const Rectangle Intersect(const Rectangle &r) const;

		// ����������, ������������ �� ������ ������������� � ��������������� r.
		inline bool IntersectsWith(const Rectangle &r) const;

		// �������� ��������� ����� �������������� �� ��������� ��������.
		inline void Offset(Point &p);
		inline void Offset(int x, int y);


		inline const Rectangle& operator=(const Rectangle& r);

		inline const bool operator==(const Rectangle& r) const;

		inline const bool operator!=(const Rectangle& r) const;

	};

	inline const Rectangle FromLTRB(const Point &LT, const Point &RB);
	inline const Rectangle FromLTRB(int LTx, int LTy, int RBx, int RBy);
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
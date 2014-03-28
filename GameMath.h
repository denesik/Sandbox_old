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

namespace gui
{
	struct Size;

	struct Point
	{
		int x;
		int y;

		Point();
		Point(int val);
		Point(int x, int y);
		Point(Point &p);
		~Point(){};

		// Смещает точку
		void Offset(Point &p);
		void Offset(int x, int y);

		// Сравнивает точку
		bool Equals(Point &p);
		bool Equals(int x, int y);

		// Возвращает true если x и y равны 0
		bool IsEmpty();


		const Point& operator=(const Point& p);

		// Смещает точку Point на заданное значение Size.
		const Point operator+(const Size& s) const;

		const Point operator-(const Size& s) const;

		const bool operator==(const Point& p) const;

		const bool operator!=(const Point& p) const;

	};

	struct Size
	{
		int width;
		int height;

		Size();
		Size(Point &p);
		Size(Size &s);
		Size(int width, int height);

		// Возвращает true если w и h равны 0
		bool IsEmpty();

		void Add(Size &s);
		void Subtract(Size &s);

		bool Equals(Size &s);
		bool Equals(int widht, int height);


		const Size& operator=(const Size& s);

		const Size operator+(const Size& s) const;

		const Size operator-(const Size& s) const; 

		const bool operator==(const Size& s) const;

		const bool operator!=(const Size& s) const;

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
		Rectangle(Point &p, Size &s);
		Rectangle(int x, int y, int w, int h);
		Rectangle(Rectangle &rectangle);

	};
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
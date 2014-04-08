#ifndef GameMath_h__
#define GameMath_h__

#include "glm.hpp"

//using namespace glm;

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

		// Смещает точку
		inline void Offset(const Point &p);
		inline void Offset(int x, int y);

		// Сравнивает точку
		inline bool Equals(const Point &p) const;
		inline bool Equals(int x, int y) const;

		// Возвращает true если x и y равны 0
		inline bool IsEmpty() const;


		inline const Point& operator=(const Point& p);

		// Смещает точку Point на заданное значение Size.
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

		// Возвращает true если w и h равны 0
		inline bool IsEmpty() const;
		
		void Add(const Size &s);
		void Subtract(const Size &s);

		inline bool Equals(const Size &s) const;
		inline bool Equals(int widht, int height) const;

		void Clamp(const Size &min, const Size &max);

		const Size& operator=(const Size& s);

		const Size operator+(const Size& s) const;

		const Size operator-(const Size& s) const; 

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

		// Возвращает true если площадь = 0
		bool IsAreaNull() const;

		inline int Bottom() const;
		inline int Top() const;
		inline int Left() const;
		inline int Right() const;

		// Определяет, содержится ли заданная точка в структуре Rectangle.
		inline bool Contains(const Point &p) const;
		inline bool Contains(int x, int y) const;
		bool Contains(const Rectangle &r) const;

		inline bool Equals(const Rectangle &r) const;

		// Увеличивает данный объект Rectangle на заданную величину.
		void Inflate(const Size &s);
		void Inflate(int w, int h);

		// Возвращает пересечение данного прямоугольника с прямоугольником r.
		Rectangle Intersect(const Rectangle &r) const;

		// Определяет, пересекается ли данный прямоугольник с прямоугольником r.
		inline bool IntersectsWith(const Rectangle &r) const;

		// Изменяет положение этого прямоугольника на указанную величину.
		inline void Offset(Point &p);
		inline void Offset(int x, int y);


		Rectangle& operator=(const Rectangle& r);

		const bool operator==(const Rectangle& r) const;

		inline const bool operator!=(const Rectangle& r) const;

	};


	struct Color
	{
		union
		{
			struct
			{
				glm::uint8 A, B, G, R;
			};

			glm::uint32 color32;
			glm::uint8 color8[4];
		};

		Color();
		Color(glm::uint32 rgba);
		Color(glm::uint8 r, glm::uint8 g, glm::uint8 b, glm::uint8 a = 0);
		Color(const Color &c);

		inline bool Equals(const Color &c) const;

		inline const Color& operator=(const Color& c);

		inline const bool operator==(const Color& c) const;

		inline const bool operator!=(const Color& c) const;

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
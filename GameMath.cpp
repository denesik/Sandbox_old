#include "GameMath.h"




gui::Point::Point( int val )
{
	x = val;
	y = val;
}

gui::Point::Point()
{
	x = 0;
	y = 0;
}

gui::Point::Point( int _x, int _y )
{
	x = _x;
	y = _y;
}

gui::Point::Point( Point &p )
{
	x = p.x;
	y = p.y;
}

void gui::Point::Offset( Point &p )
{
	x += p.x;
	y += p.y;
}

void gui::Point::Offset( int _x, int _y )
{
	x += _x;
	y += _y;
}

bool gui::Point::Equals( Point &p )
{
	return x == p.x && y == p.y;
}

bool gui::Point::Equals( int _x, int _y )
{
	return x == _x && y == _y;
}

bool gui::Point::IsEmpty()
{
	return x == 0 && y == 0;
}

const gui::Point gui::Point::operator+( const Size& s ) const
{
	return Point(x + s.width, y + s.height);
}

const gui::Point gui::Point::operator-( const Size& s ) const
{
	return Point(x - s.width, y - s.height);
}

const bool gui::Point::operator==( const Point& p ) const
{
	return x == p.x && y == p.y;
}

const bool gui::Point::operator!=( const Point& p ) const
{
	return x != p.x || y != p.y;
}

const gui::Point& gui::Point::operator=( const Point& p )
{
	x = p.x;
	y = p.y;
	return *this;
}

gui::Size::Size( Point &p )
{
	width = p.x;
	height = p.y;
}

gui::Size::Size()
{
	width = 0;
	height = 0;
}

gui::Size::Size( int _width, int _height )
{
	width = _width;
	height = _height;
}

gui::Size::Size( Size &s )
{
	width = s.width;
	height = s.height;
}

bool gui::Size::IsEmpty()
{
	return width == 0 && height == 0;
}

void gui::Size::Add( Size &s )
{
	width += s.width;
	height += s.height;
}

void gui::Size::Subtract( Size &s )
{
	width -= s.width;
	height -= s.height;
}

bool gui::Size::Equals( Size &s )
{
	return width == s.width && height == s.height;
}

bool gui::Size::Equals( int _width, int _height )
{
	return width == _width && height == _height;
}

const gui::Size& gui::Size::operator=( const Size& s )
{
	width = s.width;
	height = s.height;
	return *this;
}

const gui::Size gui::Size::operator+( const Size& s ) const
{
	return Size(width + s.width, height + s.height);
}

const gui::Size gui::Size::operator-( const Size& s ) const
{
	return Size(width - s.width, height - s.height);
}

const bool gui::Size::operator==( const Size& s ) const
{
	return width == s.width && height == s.height;
}

const bool gui::Size::operator!=( const Size& s ) const
{
	return width != s.width || height != s.height;
}



gui::Rectangle::Rectangle()
{
	x = 0;
	y = 0;
	w = 0;
	h = 0;
}

gui::Rectangle::Rectangle( Point &p, Size &s )
{
	pos = p;
	size = s;
}

gui::Rectangle::Rectangle( int _x, int _y, int _w, int _h )
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

gui::Rectangle::Rectangle( Rectangle &rectangle )
{
	pos = rectangle.pos;
	size = rectangle.size;
}

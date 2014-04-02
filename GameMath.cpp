#include "GameMath.h"


namespace gm
{

Point::Point( int val )
{
	x = val;
	y = val;
}

Point::Point()
{
	x = 0;
	y = 0;
}

Point::Point( int _x, int _y )
{
	x = _x;
	y = _y;
}

Point::Point( const Point &p )
{
	x = p.x;
	y = p.y;
}

void Point::Offset( const Point &p )
{
	x += p.x;
	y += p.y;
}

void Point::Offset( int _x, int _y )
{
	x += _x;
	y += _y;
}

bool Point::Equals( const Point &p ) const
{
	return x == p.x && y == p.y;
}

bool Point::Equals( int _x, int _y ) const
{
	return x == _x && y == _y;
}

bool Point::IsEmpty() const
{
	return x == 0 && y == 0;
}

const Point Point::operator+( const Size& s ) const
{
	return Point(x + s.width, y + s.height);
}

const Point Point::operator-( const Size& s ) const
{
	return Point(x - s.width, y - s.height);
}

const bool Point::operator==( const Point& p ) const
{
	return x == p.x && y == p.y;
}

const bool Point::operator!=( const Point& p ) const
{
	return x != p.x || y != p.y;
}

const Point& Point::operator=( const Point& p )
{
	x = p.x;
	y = p.y;
	return *this;
}

Size::Size( const Point &p )
{
	width = p.x;
	height = p.y;
}

Size::Size()
{
	width = 0;
	height = 0;
}

Size::Size( int _width, int _height )
{
	width = _width;
	height = _height;
}

Size::Size( const Size &s )
{
	width = s.width;
	height = s.height;
}

bool Size::IsEmpty() const
{
	return width == 0 && height == 0;
}

void Size::Add(const Size &s)
{
	width += s.width;
	height += s.height;
}

void Size::Subtract( const Size &s )
{
	width -= s.width;
	height -= s.height;
}

bool Size::Equals( const Size &s ) const
{
	return width == s.width && height == s.height;
}

bool Size::Equals( int _width, int _height ) const
{
	return width == _width && height == _height;
}

const Size& Size::operator=( const Size& s )
{
	width = s.width;
	height = s.height;
	return *this;
}

const Size Size::operator+( const Size& s ) const
{
	return Size(width + s.width, height + s.height);
}

const Size Size::operator-( const Size& s ) const
{
	return Size(width - s.width, height - s.height);
}

const bool Size::operator==( const Size& s ) const
{
	return width == s.width && height == s.height;
}

const bool Size::operator!=( const Size& s ) const
{
	return width != s.width || height != s.height;
}

void Size::Clamp( const Size &min, const Size &max )
{
	if(width > max.width) 
		width = max.width;
	else
		if(width < min.width) 
			width = min.width;
	if(height > max.height) 
		height = max.height;
	else
		if(height < min.height) 
			height = min.height;
}


Rectangle::Rectangle()
{
	x = 0;
	y = 0;
	w = 0;
	h = 0;
}

Rectangle::Rectangle( const Point &p, const Size &s )
{
	pos = p;
	size = s;
}

Rectangle::Rectangle( int _x, int _y, int _w, int _h )
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

Rectangle::Rectangle( const Rectangle &rectangle )
{
	pos = rectangle.pos;
	size = rectangle.size;
}

bool Rectangle::IsEmpty() const
{
	return pos.IsEmpty() && size.IsEmpty();
}

int Rectangle::Bottom() const
{
	return h >= 0 ? y + h : y;
}

int Rectangle::Top() const
{
	return h >= 0 ? y : y + h;
}

int Rectangle::Left() const
{
	return w >= 0 ? x : x + w;
}

int Rectangle::Right() const
{
	return w >= 0 ? x + w : x;
}

bool Rectangle::Contains( const Point &p ) const
{
	return p.x >= Left() && p.x <= Right() && p.y >= Top() && p.y <= Bottom();
}

bool Rectangle::Contains( int x, int y ) const
{
	return x >= Left() && x <= Right() && y >= Top() && y <= Bottom();
}

bool Rectangle::Contains( const Rectangle &r ) const
{
	return Contains(r.pos) && Contains(r.pos + r.size);
}

bool Rectangle::Equals( const Rectangle &r ) const
{
	return pos.Equals(r.pos) && size.Equals(r.size);
}

void Rectangle::Inflate( const Size &s )
{
	size.Add(s);
}

void Rectangle::Inflate( int w, int h )
{
	size.width += w;
	size.height += h;
}

void Rectangle::Offset( Point &p )
{
	pos.Offset(p);
}

void Rectangle::Offset( int x, int y )
{
	pos.Offset(x, y);
}

Rectangle& Rectangle::operator=( const Rectangle& r )
{
	pos = r.pos;
	size = r.size;
	return *this;
}

const bool Rectangle::operator==( const Rectangle& r ) const
{
	return pos == r.pos && size == r.size;
}

const bool Rectangle::operator!=( const Rectangle& r ) const
{
	return pos != r.pos || size != r.size;
}

Rectangle Rectangle::Intersect( const Rectangle &r ) const
{
	if( !IntersectsWith(r) )
		return Rectangle();

	Rectangle LT = FromLTRB(Left(), Top(), r.Left(), r.Top());
	Rectangle RB = FromLTRB(Right(), Bottom(), r.Right(), r.Bottom());

	return FromLTRB(LT.Right(), LT.Bottom(), RB.Left(), RB.Top());
}

bool Rectangle::IntersectsWith( const Rectangle &r ) const
{
	return Contains(r.pos) || r.Contains(pos);
}

bool Rectangle::IsAreaNull() const
{
	return w == 0 || h == 0;
}


const Rectangle FromLTRB( const Point &LT, const Point &RB )
{
	return Rectangle(LT.x, LT.y, RB.x - LT.x, RB.y - LT.y);
}

const Rectangle FromLTRB( int LTx, int LTy, int RBx, int RBy )
{
	return Rectangle(LTx, LTy, RBx - LTx, RBy - LTy);
}


Color::Color()
{
	color32 = 0;
}

Color::Color( glm::uint32 rgba )
{
	color32 = rgba;
}

Color::Color( glm::uint8 r, glm::uint8 g, glm::uint8 b, glm::uint8 a /*= 0*/ )
{
	R = r;
	G = g;
	B = b;
	A = a;
}

Color::Color( const Color &c )
{
	color32 = c.color32;
}

bool Color::Equals( const Color &c ) const
{
	return color32 == c.color32;
}

const Color& Color::operator=( const Color& c )
{
	color32 = c.color32;
	return *this;
}

const bool Color::operator==( const Color& c ) const
{
	return color32 == c.color32;
}

const bool Color::operator!=( const Color& c ) const
{
	return color32 != c.color32;
}


}
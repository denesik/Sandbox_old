#include "GameMath.h"




gm::Point::Point( int val )
{
	x = val;
	y = val;
}

gm::Point::Point()
{
	x = 0;
	y = 0;
}

gm::Point::Point( int _x, int _y )
{
	x = _x;
	y = _y;
}

gm::Point::Point( const Point &p )
{
	x = p.x;
	y = p.y;
}

void gm::Point::Offset( const Point &p )
{
	x += p.x;
	y += p.y;
}

void gm::Point::Offset( int _x, int _y )
{
	x += _x;
	y += _y;
}

bool gm::Point::Equals( const Point &p ) const
{
	return x == p.x && y == p.y;
}

bool gm::Point::Equals( int _x, int _y ) const
{
	return x == _x && y == _y;
}

bool gm::Point::IsEmpty() const
{
	return x == 0 && y == 0;
}

const gm::Point gm::Point::operator+( const Size& s ) const
{
	return Point(x + s.width, y + s.height);
}

const gm::Point gm::Point::operator-( const Size& s ) const
{
	return Point(x - s.width, y - s.height);
}

const bool gm::Point::operator==( const Point& p ) const
{
	return x == p.x && y == p.y;
}

const bool gm::Point::operator!=( const Point& p ) const
{
	return x != p.x || y != p.y;
}

const gm::Point& gm::Point::operator=( const Point& p )
{
	x = p.x;
	y = p.y;
	return *this;
}

gm::Size::Size( const Point &p )
{
	width = p.x;
	height = p.y;
}

gm::Size::Size()
{
	width = 0;
	height = 0;
}

gm::Size::Size( int _width, int _height )
{
	width = _width;
	height = _height;
}

gm::Size::Size( const Size &s )
{
	width = s.width;
	height = s.height;
}

bool gm::Size::IsEmpty() const
{
	return width == 0 && height == 0;
}

void gm::Size::Add( const Size &s )
{
	width += s.width;
	height += s.height;
}

void gm::Size::Subtract( const Size &s )
{
	width -= s.width;
	height -= s.height;
}

bool gm::Size::Equals( const Size &s ) const
{
	return width == s.width && height == s.height;
}

bool gm::Size::Equals( int _width, int _height ) const
{
	return width == _width && height == _height;
}

const gm::Size& gm::Size::operator=( const Size& s )
{
	width = s.width;
	height = s.height;
	return *this;
}

const gm::Size gm::Size::operator+( const Size& s ) const
{
	return Size(width + s.width, height + s.height);
}

const gm::Size gm::Size::operator-( const Size& s ) const
{
	return Size(width - s.width, height - s.height);
}

const bool gm::Size::operator==( const Size& s ) const
{
	return width == s.width && height == s.height;
}

const bool gm::Size::operator!=( const Size& s ) const
{
	return width != s.width || height != s.height;
}



gm::Rectangle::Rectangle()
{
	x = 0;
	y = 0;
	w = 0;
	h = 0;
}

gm::Rectangle::Rectangle( const Point &p, const Size &s )
{
	pos = p;
	size = s;
}

gm::Rectangle::Rectangle( int _x, int _y, int _w, int _h )
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

gm::Rectangle::Rectangle( const Rectangle &rectangle )
{
	pos = rectangle.pos;
	size = rectangle.size;
}

bool gm::Rectangle::IsEmpty() const
{
	return pos.IsEmpty() && size.IsEmpty();
}

int gm::Rectangle::Bottom() const
{
	return h >= 0 ? y + h : y;
}

int gm::Rectangle::Top() const
{
	return h >= 0 ? y : y + h;
}

int gm::Rectangle::Left() const
{
	return w >= 0 ? x : x + w;
}

int gm::Rectangle::Right() const
{
	return w >= 0 ? x + w : x;
}

bool gm::Rectangle::Contains( const Point &p ) const
{
	return p.x >= Left() && p.x <= Right() && p.y >= Top() && p.y <= Bottom();
}

bool gm::Rectangle::Contains( int x, int y ) const
{
	return x >= Left() && x <= Right() && y >= Top() && y <= Bottom();
}

bool gm::Rectangle::Contains( const Rectangle &r ) const
{
	return Contains(r.pos) && Contains(r.pos + r.size);
}

bool gm::Rectangle::Equals( const Rectangle &r ) const
{
	return pos.Equals(r.pos) && size.Equals(r.size);
}

void gm::Rectangle::Inflate( const Size &s )
{
	size.Add(s);
}

void gm::Rectangle::Inflate( int w, int h )
{
	size.width += w;
	size.height += h;
}

void gm::Rectangle::Offset( Point &p )
{
	pos.Offset(p);
}

void gm::Rectangle::Offset( int x, int y )
{
	pos.Offset(x, y);
}

const gm::Rectangle& gm::Rectangle::operator=( const Rectangle& r )
{
	pos = r.pos;
	size = r.size;
	return *this;
}

const bool gm::Rectangle::operator==( const Rectangle& r ) const
{
	return pos == r.pos && size == r.size;
}

const bool gm::Rectangle::operator!=( const Rectangle& r ) const
{
	return pos != r.pos || size != r.size;
}

const gm::Rectangle gm::Rectangle::Intersect( const Rectangle &r ) const
{
	if( !IntersectsWith(r) )
		return Rectangle();

	Rectangle LT = FromLTRB(Left(), Top(), r.Left(), r.Top());
	Rectangle RB = FromLTRB(Right(), Bottom(), r.Right(), r.Bottom());

	return gm::FromLTRB(LT.Right(), LT.Bottom(), RB.Left(), RB.Top());
}

bool gm::Rectangle::IntersectsWith( const Rectangle &r ) const
{
	return Contains(r.pos) && r.Contains(pos);
}


const gm::Rectangle gm::FromLTRB( const Point &LT, const Point &RB )
{
	return Rectangle(LT.x, LT.y, RB.x, RB.y);
}

const gm::Rectangle gm::FromLTRB( int LTx, int LTy, int RBx, int RBy )
{
	return Rectangle(LTx, LTy, RBx, RBy);
}


gm::Color::Color()
{
	color32 = 0;
}

gm::Color::Color( glm::uint32 rgba )
{
	color32 = rgba;
}

gm::Color::Color( glm::uint8 r, glm::uint8 g, glm::uint8 b, glm::uint8 a /*= 0*/ )
{
	R = r;
	G = g;
	B = b;
	A = a;
}

gm::Color::Color( const Color &c )
{
	color32 = c.color32;
}

bool gm::Color::Equals( const Color &c ) const
{
	return color32 == c.color32;
}

const gm::Color& gm::Color::operator=( const Color& c )
{
	color32 == c.color32;
	return *this;
}

const bool gm::Color::operator==( const Color& c ) const
{
	return color32 == c.color32;
}

const bool gm::Color::operator!=( const Color& c ) const
{
	return color32 != c.color32;
}

#include "Mouse.h"


GLFWwindow *Mouse::window;

double Mouse::xpos;
double Mouse::ypos;

double Mouse::dxpos;
double Mouse::dypos;

double Mouse::xposCentral;
double Mouse::yposCentral;

void Mouse::Init( GLFWwindow *win )
{
	window = win;
	xpos = 0.0;
	ypos = 0.0;
	dxpos = 0.0;
	dypos = 0.0;
	xposCentral = 0.0;
	yposCentral = 0.0;
}


void Mouse::SetButton( int button )
{

}

void Mouse::SetCursorPos( double x, double y )
{
	dxpos = x - xpos;
	dypos = y - ypos;

	// устанавливаем курсор в центр экрана
	xpos = xposCentral;
	ypos = yposCentral;
	glfwSetCursorPos(window, xposCentral, yposCentral);
}

void Mouse::SetCursorPosCentral( double x, double y )
{
	xpos = xposCentral = x;
	ypos = yposCentral = y;
}

double Mouse::IsMoveCursorX()
{
	double dx = dxpos;
	dxpos = 0.0;
	return dx;
}

double Mouse::IsMoveCursorY()
{
	double dy = dypos;
	dypos = 0.0;
	return dy;
}

void Mouse::GetCursorPos( double &x, double &y )
{
	x = xpos;
	y = ypos;
}

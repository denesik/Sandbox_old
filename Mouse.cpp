#include "Mouse.h"
#include "Logger.h"


GLFWwindow *Mouse::window;

double Mouse::xpos;
double Mouse::ypos;

double Mouse::dxpos;
double Mouse::dypos;

unsigned int Mouse::windowWidth;
unsigned int Mouse::windowHeight;

bool Mouse::stateFixedMousePos;
bool Mouse::isCursorClientArea;
bool Mouse::isWindowFocused;

void Mouse::Init( GLFWwindow *win )
{
	window = win;
	xpos = 1.0;
	ypos = 1.0;
	dxpos = 0.0;
	dypos = 0.0;
	windowWidth = 0;
	windowHeight = 0;
	stateFixedMousePos = false;
	isCursorClientArea = true;
	isWindowFocused = true;

	glfwSetCursorPos(window, xpos, ypos);
}


void Mouse::SetButton( int button )
{

}

void Mouse::SetCursorPos( double x, double y )
{
	if(!isWindowFocused)
		return;

	dxpos = x - xpos;
	dypos = y - ypos;

	if(!isCursorClientArea)
	{
		dxpos = 0.0;
		dypos = 0.0;
	}

	if(stateFixedMousePos)
	{
		// устанавливаем курсор в центр экрана
		xpos = double(windowWidth) / 2.0;
		ypos = double(windowHeight) / 2.0;
		glfwSetCursorPos(window, xpos, ypos);
	}
	else
	{
		xpos = x;
		ypos = y;
	}
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

void Mouse::SetFixedPosState( bool state )
{
	stateFixedMousePos = state;
	if(stateFixedMousePos)
	{
		xpos = double(windowWidth) / 2.0;
		ypos = double(windowHeight) / 2.0;
		dxpos = 0.0;
		dypos = 0.0;
		glfwSetCursorPos(window, xpos, ypos);
	}
}

void Mouse::SetWindowSize( unsigned int width, unsigned int height )
{
	windowWidth = width;
	windowHeight = height;
}

void Mouse::CursorClientArea( int entered )
{
	if(entered == GL_TRUE)
	{
		isCursorClientArea = true;
		glfwGetCursorPos(window, &xpos, &ypos);
	}
	else
	{
		isCursorClientArea = false;
	}
}

void Mouse::WindowFocus( int focused )
{
	if(focused == GL_TRUE)
	{
		isWindowFocused = true;
		glfwGetCursorPos(window, &xpos, &ypos);
	}
	else
	{
		isWindowFocused = false;
	}
}


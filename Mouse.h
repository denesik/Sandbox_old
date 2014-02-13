#ifndef Mouse_h__
#define Mouse_h__

#include <glfw3.h>

class Mouse
{
private:
	static GLFWwindow *window;

	static double xpos;
	static double ypos;

	static double dxpos;
	static double dypos;

	static unsigned int windowWidth;
	static unsigned int windowHeight;

	static bool stateFixedMousePos;
	static bool isCursorClientArea;
	static bool isWindowFocused;

public:

	static void CursorClientArea(int entered);
	static void SetButton(int button);
	static void SetCursorPos(double xpos, double ypos);
	static void WindowFocus(int focused);

	static void Init( GLFWwindow *win );

	static void SetFixedPosState( bool state);

	static bool GetFixedPosState()
	{
		return stateFixedMousePos;
	}

	static void SetWindowSize(unsigned int width, unsigned int height);

	static void GetCursorPos(double &xpos, double &ypos);

	static double IsMoveCursorX();
	static double IsMoveCursorY();
};


#endif // Mouse_h__
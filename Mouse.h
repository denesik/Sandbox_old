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

	static double xposCentral;
	static double yposCentral;

public:

	static void Init( GLFWwindow *win );

	static void SetButton(int button);
	static void SetCursorPos(double xpos, double ypos);

	static void SetCursorPosCentral(double xpos, double ypos);

	static void GetCursorPos(double &xpos, double &ypos);

	static double IsMoveCursorX();
	static double IsMoveCursorY();
};


#endif // Mouse_h__
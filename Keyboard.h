#ifndef Keyboard_h__
#define Keyboard_h__

#include <glfw3.h>

class Keyboard
{
private:
	static int keys[GLFW_KEY_LAST];

public:

	static void Init();

	static void SetKey(int key, int scancode, int action, int mods);

	static bool isKeyPress(int key);
	static bool isKeyUp(int key);
	static bool isKeyDown(int key);

};


#endif // Keyboard_h__
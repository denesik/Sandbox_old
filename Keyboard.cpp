#include "Keyboard.h"

int Keyboard::keys[GLFW_KEY_LAST];

void Keyboard::Init()
{
	for(unsigned int i = 0; i < GLFW_KEY_LAST; i++)
	{
		keys[i] = GLFW_RELEASE;
	}
}


void Keyboard::SetKey( int key, int scancode, int action, int mods )
{
	keys[key] = action;
}

bool Keyboard::isKeyPress( int key )
{
	if(keys[key] == GLFW_PRESS)
	{
		keys[key] = GLFW_REPEAT;
		return true;
	}
	return false;
}

bool Keyboard::isKeyUp( int key )
{
	if(keys[key] == GLFW_RELEASE)
		return true;
	return false;
}

bool Keyboard::isKeyDown( int key )
{
	if(keys[key] == GLFW_REPEAT || keys[key] == GLFW_PRESS)
		return true;
	return false;
}

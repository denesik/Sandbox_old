#include "FPSCounter.h"

#include <glfw3.h>


FPSCounter::FPSCounter(void)
{
	currentTime = glfwGetTime();
	lastTime = currentTime;
	fpsTime = 0;
}


FPSCounter::~FPSCounter(void)
{
}

void FPSCounter::Update()
{
	currentTime = glfwGetTime();
	double frameTime = currentTime - lastTime;

	fpsTime += frameTime;
	fpsStack.push_back(frameTime);

	while(fpsTime > 1)
	{
		fpsTime -= fpsStack.front();
		fpsStack.pop_front();
	}

	lastTime = currentTime;
}

unsigned int FPSCounter::GetCount()
{
	return fpsStack.size();
}

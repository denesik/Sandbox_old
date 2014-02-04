#ifndef Game_h__
#define Game_h__

//#define GLFW_INCLUDE_GLU
#include <glfw3.h>

class Game
{
protected:

	bool Running;

	const char* title;
	unsigned int width;
	unsigned int height;
	bool fullscreen;

	GLFWwindow* window;

public:
	Game(void);
	~Game(void);

	int Run();

	virtual bool Initialize();
	virtual void LoadContent(); 
	virtual void Update();
	virtual void Draw();
	virtual void ResizeWindow(unsigned int width, unsigned int height);
	virtual void UnloadContent(); 

};


#endif // Game_h__


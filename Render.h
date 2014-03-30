#ifndef Render_h__
#define Render_h__

#include <glew.h>
#include <glfw3.h>
#include "GameMath.h"
#include <vector>
#include "BufferArray.h"

using namespace glm;

class Render
{
private:
	mat4 orthoProjection;

	unsigned int width;
	unsigned int height;

public:
	Render(void);
	~Render(void);

	bool Init();

	void SetWindowSize(unsigned int width, unsigned int height);

	mat4 GetOrthoProjection();

};


#endif // Render_h__
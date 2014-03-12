#include "Render.h"
#include "Logger.h"

#define GLM_FORCE_RADIANS
#include <gtc/matrix_transform.hpp>
#include <vector>

Render::Render(void)
{
	width = 0;
	height = 0;
	orthoProjection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, 0.1f, 100.0f);
}


Render::~Render(void)
{
}

bool Render::Init()
{

	glewExperimental=true; // Needed in core profile

	if (glewInit() != GLEW_OK) 
	{
		//LOG(LOG_ERROR, "GLEW не инициализирован.");
		return false;
	}
	glGetError();

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	
	return true;
}




void Render::SetWindowSize( unsigned int _width, unsigned int _height )
{
	width = _width;
	height = _height;
}

glm::mat4 Render::GetOrthoProjection()
{
	orthoProjection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, 0.1f, 100.0f);
	return orthoProjection;
}
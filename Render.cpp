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

	return true;
}


unsigned int Render::CreateVertexArrayObject()
{
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	return VertexArrayID;
}

unsigned int Render::CreateBufferVertex(const ArrayVertex &array)
{

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(array[0]) * array.size(), &array[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(BUFFER_VERTEX);
	glVertexAttribPointer
	(
		BUFFER_VERTEX,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		0            // array buffer offset
	);

	return buffer;
}

unsigned int Render::CreateBufferColor(const ArrayColor &array)
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(array[0]) * array.size(), &array[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(BUFFER_COLOR);
	glVertexAttribPointer
	(
		BUFFER_COLOR,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		0            // array buffer offset
	);

	return buffer;
}

unsigned int Render::CreateBufferTextCoord(const ArrayTextureCoord &array)
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	unsigned int error = glGetError();

	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(array[0]) * array.size(), &array[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(BUFFER_TEXTURE_COORD);
	glVertexAttribPointer
	(
		BUFFER_TEXTURE_COORD,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		0            // array buffer offset
	);

	return buffer;
}

unsigned int Render::CreateBufferIndex(const ArrayIndex &array)
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(array[0]) * array.size(), &array[0], GL_STATIC_DRAW);

	return buffer;
}

void Render::DeleteBufferVertex( unsigned int buffer )
{
	glDisableVertexAttribArray(BUFFER_VERTEX);
	glDeleteBuffers(1, &buffer);
}

void Render::DeleteBufferColor( unsigned int buffer )
{
	glDisableVertexAttribArray(BUFFER_COLOR);
	glDeleteBuffers(1, &buffer);
}


void Render::DeleteBufferTextCoord( unsigned int buffer )
{
	glDisableVertexAttribArray(BUFFER_TEXTURE_COORD);
	glDeleteBuffers(1, &buffer);
}

void Render::DeleteBufferIndex( unsigned int buffer )
{
	glDeleteBuffers(1, &buffer);
}

void Render::DeleteVertexArrayObject( unsigned int vao )
{
	glDeleteVertexArrays(1, &vao);
}

void Render::UseVertexArrayObject( unsigned int vao )
{
	glBindVertexArray(vao);
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

void Render::DrawBufferIndex( const ArrayIndex &array )
{
	glDrawElements(GL_TRIANGLES, sizeof(array[0]) * array.size(), GL_UNSIGNED_INT, NULL);
}

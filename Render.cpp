#include "Render.h"
#include "Logger.h"


Render::Render(void)
{
}


Render::~Render(void)
{
}

bool Render::Init()
{

	glewExperimental=true; // Needed in core profile
	if (glewInit() != GLEW_OK) 
	{
		LOG(LOG_ERROR, "GLEW не инициализирован.");
		return false;
	}

	return true;
}

unsigned int Render::CreateBufferVertex(size_t size, const void* data)
{

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

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

void Render::DeleteBufferVertex( unsigned int buffer )
{
	glDisableVertexAttribArray(BUFFER_VERTEX);
	glDeleteBuffers(1, &buffer);
}

unsigned int Render::CreateBufferColor( size_t size, const void* data )
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(BUFFER_COLOR);
	glVertexAttribPointer
	(
		BUFFER_COLOR,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		0            // array buffer offset
	);

	return buffer;
}

void Render::DeleteBufferColor( unsigned int buffer )
{
	glDisableVertexAttribArray(BUFFER_COLOR);
	glDeleteBuffers(1, &buffer);
}

unsigned int Render::CreateBufferTextCoord( size_t size, const void* data )
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, BUFFER_TEXTURE_COORD);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

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

unsigned int Render::CreateBufferIndex( size_t size, const void* data )
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	return buffer;
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

unsigned int Render::CreateVertexArray()
{
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	return VertexArrayID;
}

void Render::DeleteVertexArray( unsigned int vao )
{
	glDeleteVertexArrays(1, &vao);
}

void Render::UseVertexArray( unsigned int vao )
{
	glBindVertexArray(vao);
}

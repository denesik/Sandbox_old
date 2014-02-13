#ifndef Render_h__
#define Render_h__

#include <glew.h>
#include <glfw3.h>
#include "GameMath.h"

enum BufferType
{
	BUFFER_VERTEX,
	BUFFER_COLOR,
	BUFFER_TEXTURE_COORD,
};

struct BufferArray
{
	unsigned int lenght;
	size_t sizeElement;
	const void *data;
};



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

	unsigned int CreateBufferVertex(const BufferArray &bufferArray);
	unsigned int CreateBufferColor(const BufferArray &bufferArray);
	unsigned int CreateBufferTextCoord(const BufferArray &bufferArray);
	unsigned int CreateBufferIndex(const BufferArray &bufferArray);

	void DeleteBufferVertex(unsigned int buffer);
	void DeleteBufferColor(unsigned int buffer);
	void DeleteBufferTextCoord(unsigned int buffer);
	void DeleteBufferIndex(unsigned int buffer);

	unsigned int CreateVertexArrayObject();
	void DeleteVertexArrayObject(unsigned int vao);
	void UseVertexArrayObject(unsigned int vao);

	void DrawBufferIndex(const BufferArray &bufferArray);

	mat4 GetOrthoProjection();

};


#endif // Render_h__
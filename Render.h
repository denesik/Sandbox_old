#ifndef Render_h__
#define Render_h__

#include <glew.h>
#include <glfw3.h>

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
public:
	Render(void);
	~Render(void);

	bool Init();

	unsigned int CreateBufferVertex(const BufferArray &bufferArray);
	unsigned int CreateBufferColor(const BufferArray &bufferArray);
	unsigned int CreateBufferTextCoord(const BufferArray &bufferArray);
	unsigned int CreateBufferIndex(const BufferArray &bufferArray);

	unsigned int CreateVertexArrayObject();

	void DeleteBufferVertex(unsigned int buffer);
	void DeleteBufferColor(unsigned int buffer);
	void DeleteBufferTextCoord(unsigned int buffer);
	void DeleteBufferIndex(unsigned int buffer);

	void DeleteVertexArrayObject(unsigned int vao);
	void UseVertexArrayObject(unsigned int vao);
};


#endif // Render_h__
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

class Render
{
public:
	Render(void);
	~Render(void);

	bool Init();

	unsigned int CreateBufferVertex(size_t size, const void* data);
	unsigned int CreateBufferColor(size_t size, const void* data);
	unsigned int CreateBufferTextCoord(size_t size, const void* data);
	unsigned int CreateBufferIndex(size_t size, const void* data);

	unsigned int CreateVertexArray();

	void DeleteBufferVertex(unsigned int buffer);
	void DeleteBufferColor(unsigned int buffer);
	void DeleteBufferTextCoord(unsigned int buffer);
	void DeleteBufferIndex(unsigned int buffer);

	void DeleteVertexArray(unsigned int vao);
	void UseVertexArray(unsigned int vao);
};


#endif // Render_h__
#ifndef Render_h__
#define Render_h__

#include <glew.h>
#include <glfw3.h>
#include "GameMath.h"
#include <vector>
#include "BufferArray.h"

//typedef GLuint BufferArray;

struct GraphicBufferArrayVTI
{
	unsigned int	arrayVertex;
	unsigned int	arrayTextureCoord;
	unsigned int	arrayIndex;
};

enum BufferType
{
	BUFFER_VERTEX,
	BUFFER_COLOR,
	BUFFER_TEXTURE_COORD,
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

	unsigned int CreateBufferVertex(const ArrayVertex &arrayVertex);
	unsigned int CreateBufferColor(const ArrayColor &arrayColor);
	unsigned int CreateBufferTextCoord(const ArrayTextureCoord &arrayTextureCoord);
	unsigned int CreateBufferIndex(const ArrayIndex &arrayIndex);

	void DeleteBufferVertex(unsigned int buffer);
	void DeleteBufferColor(unsigned int buffer);
	void DeleteBufferTextCoord(unsigned int buffer);
	void DeleteBufferIndex(unsigned int buffer);

	GraphicBufferArrayVTI CreateBufferArrayVTI(const BufferArrayVTI &bufferArray);
	void DeleteBufferArrayVTI(const GraphicBufferArrayVTI &graphicBufferArray);

	unsigned int CreateVertexArrayObject();
	void DeleteVertexArrayObject(unsigned int vao);
	void UseVertexArrayObject(unsigned int vao);

	void DrawBufferIndex( const ArrayIndex &arrayIndex );

	mat4 GetOrthoProjection();

};


#endif // Render_h__
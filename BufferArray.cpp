#include "BufferArray.h"

#include <glew.h>
#include <glfw3.h>
#include "Logger.h"

BufferArray::BufferArray()
{
	activeBuffers.set(BUFFER_TYPE_VERTEX);
	stride = 0;
	VAO = 0;
	videoVertexBuffer = 0;
	videoindexBuffer = 0;

	vbSize = 0;
	ibSize = 0;
}


void BufferArray::Create( bool color, bool textcoord, bool normale, unsigned int sizeVertex /*= 0*/, unsigned int sizeIndex /*= 0*/ )
{
	vbSize = 0;
	ibSize = 0;

	activeBuffers.set(BUFFER_TYPE_VERTEX);
	stride = 0;
	VAO = 0;
	videoVertexBuffer = 0;
	videoindexBuffer = 0;

	stride += 3;	// xyz

	if(color)
	{
		activeBuffers.set(BUFFER_TYPE_COLOR);
		stride += 4;				// rgba
	}
	if(textcoord)	
	{
		activeBuffers.set(BUFFER_TYPE_TEXTCOORD);
		stride += 2;				// uv
	}
	if(normale)		
	{
		activeBuffers.set(BUFFER_TYPE_NORMALE);
		stride += 3;				// 
	}

	if(sizeVertex)
	{
		vertexBuffer.reserve(stride * sizeVertex);
	}
	if(sizeIndex)
	{
		indexBuffer.reserve(sizeIndex);
	}
}


BufferArray::~BufferArray()
{
}

void BufferArray::CreateVideoBuffer()
{
	// создаем VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Создаем буфер вершин
	glGenBuffers(1, &videoVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, videoVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer[0]) * vbSize, &vertexBuffer[0], GL_STATIC_DRAW);

	unsigned int count = 0;
	if( activeBuffers.test(BUFFER_TYPE_VERTEX) )
	{
		glEnableVertexAttribArray(BUFFER_TYPE_VERTEX);
		glVertexAttribPointer
			(
			BUFFER_TYPE_VERTEX,				// attribute. No particular reason for 0, but must match the layout in the shader.
			3,								// size
			GL_FLOAT,						// type
			GL_FALSE,						// normalized?
			sizeof(float) * stride,			// stride
			0								// array buffer offset
			);
		count += 3;
	}

	if( activeBuffers.test(BUFFER_TYPE_COLOR) )
	{
		glEnableVertexAttribArray(BUFFER_TYPE_COLOR);
		glVertexAttribPointer
			(
			BUFFER_TYPE_COLOR,				// attribute. No particular reason for 0, but must match the layout in the shader.
			4,								// size
			GL_FLOAT,						// type
			GL_FALSE,						// normalized?
			sizeof(float) * stride,			// stride
			(void*)(sizeof(float) * count)	// array buffer offset
			);
		count += 4;
	}

	if( activeBuffers.test(BUFFER_TYPE_TEXTCOORD) )
	{
		glEnableVertexAttribArray(BUFFER_TYPE_TEXTCOORD);
		glVertexAttribPointer
			(
			BUFFER_TYPE_TEXTCOORD,				// attribute. No particular reason for 0, but must match the layout in the shader.
			2,									// size
			GL_FLOAT,							// type
			GL_FALSE,							// normalized?
			sizeof(float) * stride,				// stride
			(void*)(sizeof(float) * count)		// array buffer offset
			);
		count += 2;
	}

	if( activeBuffers.test(BUFFER_TYPE_NORMALE) )
	{
		glEnableVertexAttribArray(BUFFER_TYPE_NORMALE);
		glVertexAttribPointer
			(
			BUFFER_TYPE_NORMALE,				// attribute. No particular reason for 0, but must match the layout in the shader.
			3,									// size
			GL_FLOAT,							// type
			GL_FALSE,							// normalized?
			sizeof(float) * stride,				// stride
			(void*)(sizeof(float) * count)		// array buffer offset
			);
		count += 3;
	}

	glGenBuffers(1, &videoindexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, videoindexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBuffer[0]) * ibSize, &indexBuffer[0], GL_STATIC_DRAW);

	OPENGL_CHECK_ERRORS();
}

void BufferArray::Draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, ibSize, GL_UNSIGNED_INT, NULL);
}

void BufferArray::DeleteVideoBuffer()
{
	if(VAO == 0)
	{
		return;
	}

	glBindVertexArray(VAO);

	if( activeBuffers.test(BUFFER_TYPE_VERTEX) )
	{
		glDisableVertexAttribArray(BUFFER_TYPE_VERTEX);
	}
	if( activeBuffers.test(BUFFER_TYPE_COLOR) )
	{
		glDisableVertexAttribArray(BUFFER_TYPE_COLOR);
	}
	if( activeBuffers.test(BUFFER_TYPE_TEXTCOORD) )
	{
		glDisableVertexAttribArray(BUFFER_TYPE_TEXTCOORD);
	}
	if( activeBuffers.test(BUFFER_TYPE_NORMALE) )
	{
		glDisableVertexAttribArray(BUFFER_TYPE_NORMALE);
	}
	
	if(videoVertexBuffer)
	{
		glDeleteBuffers(1, &videoVertexBuffer);
	}
	if(videoindexBuffer)
	{
		glDeleteBuffers(1, &videoindexBuffer);
	}
	glDeleteVertexArrays(1, &VAO);

	OPENGL_CHECK_ERRORS();
}


void BufferArray::PushBack( BufferArray &ba )
{
	if(ba.activeBuffers != activeBuffers)
	{
		LOG_ERROR("Типы буферов не совместимы.");
		return;
	}

	if(ba.vertexBuffer.size() < ba.vbSize ||
	   ba.indexBuffer.size() < ba.ibSize)
	{
		LOG_ERROR("Попытка вставить некорректный буфер.");
		return;
	}

	unsigned int vertexCount = vbSize / stride;

	for(unsigned int i = 0; i < ba.vbSize; i++)
	{
		if(vertexBuffer.size() > vbSize + i)
		{
			vertexBuffer[vbSize + i] = ba.vertexBuffer[i];
		}
		else
		{
			vertexBuffer.push_back(ba.vertexBuffer[i]);
		}
	}
	vbSize += ba.vbSize;

	for(unsigned int i = 0; i < ba.ibSize; i++)
	{
		if(indexBuffer.size() > ibSize + i)
		{
			indexBuffer[ibSize + i] = ba.indexBuffer[i] + vertexCount;
		}
		else
		{
			indexBuffer.push_back(ba.indexBuffer[i] + vertexCount);
		}
	}

	ibSize += ba.ibSize;
	
}

void BufferArray::Clear()
{
	vertexBuffer.clear();
	indexBuffer.clear();
	vbSize = 0;
	ibSize = 0;
}

void BufferArray::Reset()
{
	vbSize = 0;
	ibSize = 0;
}

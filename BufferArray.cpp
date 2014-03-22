#include "BufferArray.h"

#include <glew.h>
#include <glfw3.h>
#include "Logger.h"


BufferArray::BufferArray(bool color, bool textcoord, bool normale)
	: kReserve(1.5f)
{
	vertexBuffer = nullptr;
	indexBuffer = nullptr;

	vbCapacity = 0;
	ibCapacity = 0;

	vbSize = 0;
	ibSize = 0;

	vaoCreated = false;

	VAO = 0;
	videoVertexBuffer = 0;
	videoindexBuffer = 0;

	activeBuffers.set(BUFFER_TYPE_VERTEX);
	stride = 3;						// xyz


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
		stride += 3;				// xyz
	}
}

BufferArray::~BufferArray()
{
}

void BufferArray::Reserve( unsigned int sizeVertex, unsigned int sizeIndex )
{
	if(sizeVertex == 0 && sizeIndex == 0)
		return;

	ReserveVertex(sizeVertex);
	ReserveIndex(sizeIndex);

	// Видео буфер тоже нужно будет пересоздать
	// vaoCreated = false;
}

void BufferArray::ReserveVertex( unsigned int count )
{
	vbCapacity += count * stride;
	float*	vb;

	vb = new float[vbCapacity];

	// перемещаем буфер вершин
	if(vertexBuffer)
	{
		for(unsigned int i = 0; i < vbSize; i++)
		{
			vb[i] = vertexBuffer[i];
		}
	}

	// Удалять nullptr можно
	delete[] vertexBuffer;

	vertexBuffer = vb;

	// Видео буфер тоже нужно будет пересоздать
	vaoCreated = false;
}

void BufferArray::ReserveIndex( unsigned int count )
{

	ibCapacity += count;
	unsigned int*	ib;

	ib = new unsigned int[ibCapacity];

	// Перемещаем индексный буфер
	if(indexBuffer)
	{
		for(unsigned int i = 0; i < ibSize; i++)
		{
			ib[i] = indexBuffer[i];
		}
	}

	// Удалять nullptr можно
	delete[] indexBuffer;

	indexBuffer = ib;

	// Видео буфер тоже нужно будет пересоздать
	vaoCreated = false;
}


void BufferArray::CreateVideoBuffer()
{
	if(!vaoCreated)
	{
		// Пытаемся удалить буфер, вдруг он не был удален
		RemoveVideoBuffer();

		// создаем VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Создаем буфер вершин
		glGenBuffers(1, &videoVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, videoVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer[0]) * vbCapacity, &vertexBuffer[0], GL_STATIC_DRAW);

		// Создаем буфер индексов
		glGenBuffers(1, &videoindexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, videoindexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBuffer[0]) * ibCapacity, &indexBuffer[0], GL_STATIC_DRAW);

		OPENGL_CHECK_ERRORS();
		vaoCreated = true;
	}
	else
	{
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, videoVertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexBuffer[0]) * vbSize, &vertexBuffer[0]);
	
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, videoindexBuffer);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indexBuffer[0]) * ibSize, &indexBuffer[0]);

		OPENGL_CHECK_ERRORS();
	}

	// Настраиваем vao
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

}

void BufferArray::RemoveVideoBuffer()
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

	unsigned int vertexCount = vbSize / stride;

	// Если памяти меньше чем нужно
	if(vbCapacity < vbSize + ba.vbSize)
	{
		if(vbCapacity == 0)
			ReserveVertex(unsigned int(ba.vbSize / stride));
		else
			ReserveVertex(unsigned int((vbCapacity / stride) * kReserve));
	}
	if(ibCapacity < ibSize + ba.ibSize)
	{
		if(ibCapacity == 0)
			ReserveIndex(unsigned int(ba.ibSize));
		else
			ReserveIndex(unsigned int(ibCapacity * kReserve));
	}

	for(unsigned int i = 0; i < ba.vbSize; i++)
	{
		vertexBuffer[i + vbSize] = ba.vertexBuffer[i];
	}
	vbSize += ba.vbSize;

	for(unsigned int i = 0; i < ba.ibSize; i++)
	{
		indexBuffer[i + ibSize] = ba.indexBuffer[i] + vertexCount;
	}

	ibSize += ba.ibSize;	
}

void BufferArray::Draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, ibSize, GL_UNSIGNED_INT, NULL);
}

void BufferArray::Reset()
{
	vbSize = 0;
	ibSize = 0;
}




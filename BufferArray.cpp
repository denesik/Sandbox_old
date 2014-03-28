#include "BufferArray.h"

#include <glew.h>
#include <glfw3.h>
#include "Logger.h"


BufferArrayRoot::BufferArrayRoot()
	: kReserve(1.5f)
{
	vertexBuffer = nullptr;
	indexBuffer = nullptr;

	vbCapacity = 0;
	ibCapacity = 0;

	vbSize = 0;
	ibSize = 0;

	vbCreated = false;
	ibCreated = false;

	VAO = 0;
	videoVertexBuffer = 0;
	videoindexBuffer = 0;
}

BufferArrayRoot::~BufferArrayRoot()
{
	delete[] vertexBuffer;
	delete[] indexBuffer;
}


void BufferArrayRoot::ReserveVertex( unsigned int count )
{
	vbCapacity += count;
	float*	vb;

	vb = new float[vbCapacity * stride];

	// перемещаем буфер вершин
	if(vertexBuffer)
	{
		for(unsigned int i = 0; i < vbSize * stride; i++)
		{
			vb[i] = vertexBuffer[i];
		}
	}

	// Удалять nullptr можно
	delete[] vertexBuffer;

	vertexBuffer = vb;

	// Видео буфер тоже нужно будет пересоздать
	vbCreated = false;
}

void BufferArrayRoot::ReserveIndex( unsigned int count )
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
	ibCreated = false;
}


void BufferArrayRoot::Draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, ibSize, GL_UNSIGNED_INT, NULL);
}

void BufferArrayRoot::CreateVertexBuffer_()
{
	glBindVertexArray(VAO);

	if(!vbCreated)
	{
		if(videoVertexBuffer)
		{
			glDeleteBuffers(1, &videoVertexBuffer);
		}

		// Создаем буфер вершин
		glGenBuffers(1, &videoVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, videoVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer[0]) * vbCapacity * stride, &vertexBuffer[0], GL_STATIC_DRAW);

		OPENGL_CHECK_ERRORS();
		vbCreated = true;
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, videoVertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexBuffer[0]) * vbSize * stride, &vertexBuffer[0]);

		OPENGL_CHECK_ERRORS();
	}	
}

void BufferArrayRoot::CreateIndexBuffer()
{
	glBindVertexArray(VAO);

	if(!ibCreated)
	{
		if(videoindexBuffer)
		{
			glDeleteBuffers(1, &videoindexBuffer);
		}

		// Создаем буфер индексов
		glGenBuffers(1, &videoindexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, videoindexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBuffer[0]) * ibCapacity, &indexBuffer[0], GL_STATIC_DRAW);

		OPENGL_CHECK_ERRORS();
		ibCreated = true;
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, videoindexBuffer);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indexBuffer[0]) * ibSize, &indexBuffer[0]);		

		OPENGL_CHECK_ERRORS();
	}	
}

void BufferArrayRoot::UpdateVertexBuffer( unsigned int begin, unsigned int count )
{
	if(videoVertexBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, videoVertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexBuffer[0]) * count * stride, &vertexBuffer[begin * stride]);

		OPENGL_CHECK_ERRORS();
	}
}

void BufferArrayRoot::UpdateIndexBuffer( unsigned int begin, unsigned int count )
{
	if(videoindexBuffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, videoindexBuffer);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indexBuffer[0]) * count, &indexBuffer[begin]);		

		OPENGL_CHECK_ERRORS();
	}
}

float * BufferArrayRoot::GetVertexData( unsigned int index )
{

	if( index < vbSize )
		return &vertexBuffer[index * stride];

	return nullptr;
}

unsigned int * BufferArrayRoot::GetIndexData( unsigned int index )
{
	if( index < ibSize )
		return &indexBuffer[index];

	return nullptr;
}


void BufferArrayRoot::InsertBackVertex( unsigned int count )
{
	if(vbSize + count > vbCapacity)
	{
		unsigned int reserveCount = unsigned int(vbCapacity * kReserve - vbCapacity);
		if(reserveCount < count)
		{
			reserveCount = count;
		}
		ReserveVertex(reserveCount);
	}

	vbSize += count;
}

void BufferArrayRoot::InsertBackIndex( unsigned int count )
{

	if(ibSize + count > ibCapacity)
	{
		unsigned int reserveCount = unsigned int(ibCapacity * kReserve - ibCapacity);
		if(reserveCount < count)
		{
			reserveCount = count;
		}
		ReserveIndex(reserveCount);
	}

	ibSize += count;
}



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

	vaoCreated = false;

	VAO = 0;
	videoVertexBuffer = 0;
	videoindexBuffer = 0;
}

BufferArrayRoot::~BufferArrayRoot()
{
}

void BufferArrayRoot::Reserve( unsigned int sizeVertex, unsigned int sizeIndex )
{
	if(sizeVertex == 0 && sizeIndex == 0)
		return;

	ReserveVertex(sizeVertex);
	ReserveIndex(sizeIndex);

	// ����� ����� ���� ����� ����� �����������
	// vaoCreated = false;
}

void BufferArrayRoot::ReserveVertex( unsigned int count )
{
	vbCapacity += count /** stride*/;
	float*	vb;

	vb = new float[vbCapacity];

	// ���������� ����� ������
	if(vertexBuffer)
	{
		for(unsigned int i = 0; i < vbSize; i++)
		{
			vb[i] = vertexBuffer[i];
		}
	}

	// ������� nullptr �����
	delete[] vertexBuffer;

	vertexBuffer = vb;

	// ����� ����� ���� ����� ����� �����������
	vaoCreated = false;
}

void BufferArrayRoot::ReserveIndex( unsigned int count )
{

	ibCapacity += count;
	unsigned int*	ib;

	ib = new unsigned int[ibCapacity];

	// ���������� ��������� �����
	if(indexBuffer)
	{
		for(unsigned int i = 0; i < ibSize; i++)
		{
			ib[i] = indexBuffer[i];
		}
	}

	// ������� nullptr �����
	delete[] indexBuffer;

	indexBuffer = ib;

	// ����� ����� ���� ����� ����� �����������
	vaoCreated = false;
}


void BufferArrayRoot::CreateVideoBuffer_()
{
	if(!vaoCreated)
	{
		// �������� ������� �����, ����� �� �� ��� ������
		RemoveVideoBuffer();

		// ������� VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// ������� ����� ������
		glGenBuffers(1, &videoVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, videoVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer[0]) * vbCapacity, &vertexBuffer[0], GL_STATIC_DRAW);

		// ������� ����� ��������
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

}

void BufferArrayRoot::RemoveVideoBuffer()
{
	if(VAO == 0)
	{
		return;
	}

	glBindVertexArray(VAO);

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
/*
void BufferArrayRoot::PushBack( BufferArrayRoot &ba )
{
	if(ba.activeBuffers != activeBuffers)
	{
		LOG_ERROR("���� ������� �� ����������.");
		return;
	}

	unsigned int vertexCount = vbSize / stride;

	// ���� ������ ������ ��� �����
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
*/
void BufferArrayRoot::Draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, ibSize, GL_UNSIGNED_INT, NULL);
}

void BufferArrayRoot::Reset()
{
	vbSize = 0;
	ibSize = 0;
}







/*

// ����������� vao
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

 **/
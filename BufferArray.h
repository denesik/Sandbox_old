#ifndef BufferArray_h__
#define BufferArray_h__

#include <vector>
#include "GameMath.h"
#include <bitset>
#include "Vertex.h"
#include "Logger.h"


class BufferArrayRoot
{
protected:
	// ����������� �������������� ������ = 1.5
	const float kReserve;

	int stride;

	bool vbCreated;
	bool ibCreated;

	unsigned int	VAO;
	unsigned int	videoVertexBuffer;
	unsigned int	videoindexBuffer;

	// ���������� ���������� ��������� � ������
	unsigned int vbCapacity;
	unsigned int ibCapacity;

	float*			vertexBuffer;
	unsigned int*	indexBuffer;

	// ���������� ��������� � ������
	unsigned int vbSize;
	unsigned int ibSize;

public:
	// ������� �����
	BufferArrayRoot();
	~BufferArrayRoot();

	// ��������������� ������ ��� count ������
	void ReserveVertex(unsigned int count);
	// ��������������� ������ ��� count ��������
	void ReserveIndex(unsigned int count);

	// �������� �����
	void Reset();

	// ��������� ������ ������ [begin, end] � ����� ������
	void UpdateVertexBuffer(unsigned int begin, unsigned int count);
	// ��������� ������ �������� [begin, end] � ����� ������
	void UpdateIndexBuffer(unsigned int begin, unsigned int count);

	// �������� ��������� �� ������ ������� � ������� index
	float *GetVertexData(unsigned int index);
	// �������� ��������� �� ������ ������� � ������� index
	unsigned int *GetIndexData(unsigned int index);

	// ����������� ������ ������
	void InsertBackVertex(unsigned int count);
	void InsertBackIndex(unsigned int count);

	unsigned int GetSizeIndex()
	{
		return ibSize;
	};
	unsigned int GetSizeVertex()
	{
		return vbSize;
	};

	void Draw();
protected:
	void CreateVertexAttribArray(){};

	// ������� ����� ������ � ����� ������
	void CreateVertexBuffer_();
	// ������� ����� �������� � ����� ������
	void CreateIndexBuffer_();
};

template <class T, class Base, class RootTList>
class BufferArrayGenerated : public Base
{
public:
	BufferArrayGenerated()
	{
		// ������� ����� ������ 1 ���
		if(CompareType<TypeAt<RootTList, 0>::Result, T>::value)
		{
			stride = VertexStride<RootTList>::value;
			// ������� vao
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			// ������������� ��������

			CreateVertexBuffer();
			CreateIndexBuffer();
			CreateVertexAttribArray();
			OPENGL_CHECK_ERRORS();
		}
	};

	~BufferArrayGenerated()
	{
		// ������� ����� ������ 1 ���
		if(CompareType<TypeAt<RootTList, 0>::Result, T>::value)
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
	};

	// ������� ����� ������ � ����� ������
	void CreateVertexBuffer()
	{
		CreateVertexBuffer_();
		CreateVertexAttribArray();
	};
	// ������� ����� �������� � ����� ������
	void CreateIndexBuffer()
	{
		CreateIndexBuffer_();
	};

protected:
	void CreateVertexAttribArray()
	{

		static_assert(CheckDublicates<RootTList>::value == 0, "Creating buffer. Found duplicate types.");

		glEnableVertexAttribArray(T::Type);
		glVertexAttribPointer
			(
			T::Type,							// attribute. No particular reason for 0, but must match the layout in the shader.
			T::Size,							// size
			GL_FLOAT,							// type
			GL_FALSE,							// normalized?
			sizeof(float) * VertexStride<RootTList>::value,				// stride
			(void*)(sizeof(float) * VertexCount<RootTList, T>::value)	// array buffer offset
			);

		OPENGL_CHECK_ERRORS();

		Base::CreateVertexAttribArray();
	}
};


template
	<
		class TList,
		class RootTList = TList,
			template <class AtomicType, class Base, class TL> class Unit = BufferArrayGenerated,
		class Root = BufferArrayRoot
	>
class BufferArray;

template
	<
		class T1,
		class T2,
		class RootTList,
		template <class, class, class> class Unit,
		class Root
	>
class BufferArray<TypeList<T1, T2>, RootTList, Unit, Root>
	: public Unit< T1, BufferArray<T2, RootTList, Unit, Root>, RootTList>
{
};

template
	<
		class T,
		class RootTList,
		template <class, class, class> class Unit,
		class Root
	>
class BufferArray<TYPELIST_1(T), RootTList, Unit, Root>
	: public Unit<T, Root, RootTList>
{
};


#endif // BufferArray_h__
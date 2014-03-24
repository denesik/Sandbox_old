#ifndef BufferArray_h__
#define BufferArray_h__

#include <vector>
#include "GameMath.h"
#include <bitset>
#include "Vertex.h"
#include "Logger.h"


// ������ ���������� ��������� � �������
template <class TList> struct VertexStride;
template <> struct VertexStride<NullType>
{
	enum { value = 0 };
};
template <class Head,class Tail>
struct VertexStride< TypeList< Head, Tail> >
{
	enum { value = Head::Size + VertexStride<Tail>::value };
};


// ������ �������� � ������ �� ���� T
template <class TList, class T> struct VertexCount;

template <class T>
struct VertexCount<NullType, T>
{
	// ��������� ������, ���� ���� ��� � ������ �����
	//enum { value = 0 };
};

template <class T, class Tail>
struct VertexCount<TypeList<T, Tail>, T>
{
	enum { value = 0 };
};

template <class Head, class Tail, class T>
struct VertexCount<TypeList<Head, Tail>, T>
{
	enum { value = Head::Size + VertexCount<Tail, T>::value };
};




class BufferArrayRoot
{
protected:
	// ����������� �������������� ������ = 1.5
	const float kReserve;

	bool vaoCreated;

	unsigned int	VAO;
	unsigned int	videoVertexBuffer;
	unsigned int	videoindexBuffer;

	// ���������� ���������� ��������� � ������
	unsigned int vbCapacity;
	unsigned int ibCapacity;

public:
	float*			vertexBuffer;
	unsigned int*	indexBuffer;

	// ���������� ��������� � ������
	unsigned int vbSize;
	unsigned int ibSize;

public:
	// ������� �����
	BufferArrayRoot();
	~BufferArrayRoot();

	// ��������������� ������. ���������� ������, ���������� ��������.
	void Reserve(unsigned int sizeVertex, unsigned int sizeIndex);

	// �������� �����
	void Reset();

	// ������� ����� � �����������
//	void CreateVideoBuffer();
	void RemoveVideoBuffer();

	// ��������� ����� � �����
//	void PushBack(BufferArrayRoot &bufferArray);

	void Draw();
protected:

	void CreateVideoBuffer_();

	void CreateVertexAttribArray(){};
	void ReserveVertex(unsigned int count);
	void ReserveIndex(unsigned int count);

};

template <class T, class Base, class RootTList>
class BufferArrayGenerated : public Base
{
public:
	void CreateVideoBuffer()
	{
		CreateVideoBuffer_();
		CreateVertexAttribArray();
	};

protected:
	void CreateVertexAttribArray()
	{

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

		LOG_INFO("TYPE: %i; VS: %i; VC: %i;", T::Type, VertexStride<RootTList>::value, VertexCount<RootTList, T>::value);

		Base::CreateVertexAttribArray();
	}
public:
	

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
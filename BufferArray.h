#ifndef BufferArray_h__
#define BufferArray_h__

#include <vector>
#include "GameMath.h"
#include <bitset>
#include "Vertex.h"
#include "Logger.h"

template <class TList> struct VertexSize;

// Для NullType длина списка равна 0
template <> struct VertexSize<NullType>
{
	enum { value = 0 };
};

// Рекурсивная формула
template <class T,class U>
struct VertexSize< TypeList< T, U> >
{
	enum { value = T::Size + VertexSize<U>::value };
};



class BufferArrayRoot
{
protected:
	// Коэффициент резервирования памяти = 1.5
	const float kReserve;

	std::bitset<4>	activeBuffers;

//	unsigned int stride;

	bool vaoCreated;

	unsigned int	VAO;
	unsigned int	videoVertexBuffer;
	unsigned int	videoindexBuffer;

	// Количество допустимых элементов в буфере
	unsigned int vbCapacity;
	unsigned int ibCapacity;

public:
	float*			vertexBuffer;
	unsigned int*	indexBuffer;

	// Количество элементов в буфере
	unsigned int vbSize;
	unsigned int ibSize;

public:
	// Создаем буфер
	BufferArrayRoot();
	~BufferArrayRoot();

	// Зарезервировать память. Количество вершин, количество индексов.
	void Reserve(unsigned int sizeVertex, unsigned int sizeIndex);

	// Обнуляем буфер
	void Reset();

	// Создаем буфер в видеопамяти
//	void CreateVideoBuffer();
	void RemoveVideoBuffer();

	// Добавляем буфер в конец
//	void PushBack(BufferArrayRoot &bufferArray);

	void Draw();
protected:

	void CreateVideoBuffer_();

	void CreateVertexAttribArray(){};
	void ReserveVertex(unsigned int count);
	void ReserveIndex(unsigned int count);

};

template <class T, class Base, int VertexStride, int VertexCount>
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
			sizeof(float) * VertexStride,		// stride
			(void*)(sizeof(float) * VertexCount)// array buffer offset
			);

		LOG_INFO("TYPE: %i; VS: %i; VC: %i;", T::Type, VertexStride, VertexCount);

		Base::CreateVertexAttribArray();
	}

};



template
	<
		class TList,
		int VertexStride = VertexSize<TList>::value,
			template <class AtomicType, class Base, int VS, int VC> class Unit = BufferArrayGenerated,
		class Root = BufferArrayRoot
	>
class BufferArray;

template
	<
		class T1,
		class T2,
		int VertexStride,
		template <class, class, int, int> class Unit,
		class Root
	>
class BufferArray<TypeList<T1, T2>, VertexStride, Unit, Root>
	: public Unit< T1, BufferArray<T2, VertexStride, Unit, Root>, VertexStride, VertexStride - VertexSize<TypeList<T1, T2>>::value>
{
};

template
	<
		class T,
		int VertexStride,
		template <class, class, int, int> class Unit,
		class Root
	>
class BufferArray<TYPELIST_1(T), VertexStride, Unit, Root>
	: public Unit<T, Root, VertexStride, VertexStride - VertexSize<TYPELIST_1(T)>::value>
{
};


#endif // BufferArray_h__
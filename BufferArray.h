#ifndef BufferArray_h__
#define BufferArray_h__

#include <vector>
#include "GameMath.h"
#include <bitset>

enum BufferType
{
	BUFFER_TYPE_VERTEX,
	BUFFER_TYPE_COLOR,
	BUFFER_TYPE_TEXTCOORD,
	BUFFER_TYPE_NORMALE,
};

class BufferArray
{
private:
	// Коэффициент резервирования памяти = 1.5
	const float kReserve;

	std::bitset<4>	activeBuffers;

	unsigned int stride;

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
	BufferArray(bool color, bool textcoord, bool normale);
	~BufferArray();

	// Зарезервировать память. Количество вершин, количество индексов.
	void Reserve(unsigned int sizeVertex, unsigned int sizeIndex);

	// Обнуляем буфер
	void Reset();

	// Создаем буфер в видеопамяти
	void CreateVideoBuffer();
	void RemoveVideoBuffer();

	// Добавляем буфер в конец
	void PushBack(BufferArray &bufferArray);

	void Draw();
private:

	void ReserveVertex(unsigned int count);
	void ReserveIndex(unsigned int count);
};


#endif // BufferArray_h__
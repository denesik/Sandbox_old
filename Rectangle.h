#ifndef Rectangle_h__
#define Rectangle_h__

#include "GameMath.h"
#include "TextureManager.h"
#include "Render.h"
#include "BufferArray.h"

class Rectangle
{
private:
	Texture texture;

	float x, y, z;
	float width;
	float height;

public:
	Rectangle(void);
	~Rectangle(void);

	void SetSize(float width, float height);
	void SetPos(const vec3 &pos);

	void SetTexture(const Texture &texture);
	Texture GetTexture();

	template <class TList>
	unsigned int InsertBackIndex(BufferArray<TList> &buffer, unsigned int v)
	{
		static_assert(Lenght<TList>::value == 2, "Unsupported format vertices.");

		// ѕровер€ем есть ли нужные нам типы в списке типов
		static_assert(Find<TList, Vertex>::value != -1, "Unsupported format vertices.");
		static_assert(Find<TList, TextCoord>::value != -1, "Unsupported format vertices.");
		
		// ћы собираемс€ добавить 6 индесков
		buffer.InsertBackIndex(6);
		unsigned int *data = buffer.GetIndexData(buffer.GetSizeIndex() - 6);

		data[0] = v + 2;
		data[1] = v + 3;
		data[2] = v + 0;
		data[3] = v + 0;
		data[4] = v + 1;
		data[5] = v + 2;

		return 0;
	};


	template <class TList>
	unsigned int InsertBackVertex(BufferArray<TList> &buffer)
	{
		static_assert(Lenght<TList>::value == 2, "Unsupported format vertices.");

		// ѕровер€ем есть ли нужные нам типы в списке типов
		static_assert(Find<TList, Vertex>::value != -1, "Unsupported format vertices.");
		static_assert(Find<TList, TextCoord>::value != -1, "Unsupported format vertices.");

		// ”знаем смещени€
		unsigned int const stride = VertexStride<TList>::value;
		unsigned int const countVertex = VertexCount<TList, Vertex>::value;
		unsigned int const countTextCoord = VertexCount<TList, TextCoord>::value;


		// ћы собираемс€ добавить 4 вершины
		buffer.InsertBackVertex(4);
		float *data = buffer.GetVertexData(buffer.GetSizeVertex() - 4);

		data[0 + countVertex] = x;
		data[1 + countVertex] = y;
		data[2 + countVertex] = z;

		data[0 + countTextCoord] = texture.u1;
		data[1 + countTextCoord] = texture.v2;

		// верх лево
		data[0 + countVertex + stride] = x;
		data[1 + countVertex + stride] = y + height;
		data[2 + countVertex + stride] = z;

		data[0 + countTextCoord + stride] = texture.u1;
		data[1 + countTextCoord + stride] = texture.v1;

		//верх право
		data[0 + countVertex + stride * 2] = x + width;
		data[1 + countVertex + stride * 2] = y + height;
		data[2 + countVertex + stride * 2] = z;

		data[0 + countTextCoord + stride * 2] = texture.u2;
		data[1 + countTextCoord + stride * 2] = texture.v1;

		//верх право
		data[0 + countVertex + stride * 3] = x + width;
		data[1 + countVertex + stride * 3] = y;
		data[2 + countVertex + stride * 3] = z;

		data[0 + countTextCoord + stride * 3] = texture.u2;
		data[1 + countTextCoord + stride * 3] = texture.v2;

		return 0;
	};

};


#endif // Rectangle_h__
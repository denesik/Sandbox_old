// #ifndef Rectangle_h__
// #define Rectangle_h__
// 
// #include "GameMath.h"
// #include "Render.h"
// #include "BufferArray.h"
// 
// class Rectangle
// {
// private:
// 	Texture texture;
// 
// 	float x, y, z;
// 	float width;
// 	float height;
// 
// public:
// 	Rectangle(void);
// 	~Rectangle(void);
// 
// 	void SetSize(float width, float height);
// 	void SetPos(const vec3 &pos);
// 
// 	void SetTexture(const Texture &texture);
// 	Texture GetTexture();
// 
// 	template <class TList>
// 	unsigned int InsertBackIndex(BufferArray<TList> &buffer, unsigned int v)
// 	{
// 		static_assert(Lenght<TList>::value == 2, "Unsupported format vertices.");
// 
// 		// ѕровер€ем есть ли нужные нам типы в списке типов
// 		static_assert(Find<TList, Vertex>::value != -1, "Unsupported format vertices.");
// 		static_assert(Find<TList, TextCoord>::value != -1, "Unsupported format vertices.");
// 		
// 		// ћы собираемс€ добавить 6 индесков
// 		buffer.InsertBackIndex(6);
// 		unsigned int *data = buffer.GetIndexData(buffer.GetSizeIndex() - 6);
// 
// 		data[0] = v + 2;
// 		data[1] = v + 3;
// 		data[2] = v + 0;
// 		data[3] = v + 0;
// 		data[4] = v + 1;
// 		data[5] = v + 2;
// 
// 		return 0;
// 	};
// 
// 
// 	template <class TList>
// 	unsigned int InsertBackVertex(BufferArray<TList> &buffer)
// 	{
// 		static_assert(Lenght<TList>::value == 2, "Unsupported format vertices.");
// 
// 		// ѕровер€ем есть ли нужные нам типы в списке типов
// 		static_assert(Find<TList, Vertex>::value != -1, "Unsupported format vertices.");
// 		static_assert(Find<TList, TextCoord>::value != -1, "Unsupported format vertices.");
// 
// 		// ћы собираемс€ добавить 4 вершины
// 		buffer.InsertBackVertex(4);
// 		float *data = buffer.GetVertexData(buffer.GetSizeVertex() - 4);
// 		
// 		VertexAccess<TList, Vertex>(data, 0).x = x;
// 		VertexAccess<TList, Vertex>(data, 0).y = y;
// 		VertexAccess<TList, Vertex>(data, 0).z = z;
// 
// 		VertexAccess<TList, TextCoord>(data, 0).u = texture.u1;
// 		VertexAccess<TList, TextCoord>(data, 0).v = texture.v2;
// 
// 		VertexAccess<TList, Vertex>(data, 1).x = x;
// 		VertexAccess<TList, Vertex>(data, 1).y = y + height;
// 		VertexAccess<TList, Vertex>(data, 1).z = z;
// 
// 		VertexAccess<TList, TextCoord>(data, 1).u = texture.u1;
// 		VertexAccess<TList, TextCoord>(data, 1).v = texture.v1;
// 
// 		VertexAccess<TList, Vertex>(data, 2).x = x + width;
// 		VertexAccess<TList, Vertex>(data, 2).y = y + height;
// 		VertexAccess<TList, Vertex>(data, 2).z = z;
// 
// 		VertexAccess<TList, TextCoord>(data, 2).u = texture.u2;
// 		VertexAccess<TList, TextCoord>(data, 2).v = texture.v1;
// 
// 		VertexAccess<TList, Vertex>(data, 3).x = x + width;
// 		VertexAccess<TList, Vertex>(data, 3).y = y;
// 		VertexAccess<TList, Vertex>(data, 3).z = z;
// 
// 		VertexAccess<TList, TextCoord>(data, 3).u = texture.u2;
// 		VertexAccess<TList, TextCoord>(data, 3).v = texture.v2;
// 		
// 		return 0;
// 	};
// 
// };
// 
// 
// #endif // Rectangle_h__
#ifndef Cube_h__
#define Cube_h__

#include "GameMath.h"
#include "Render.h"
#include "ImageAtlas.h"


#define VERTEXCOUNT 24
#define INDEXCOUNT 36

static const float __vertexPositions[VERTEXCOUNT][3] = 
{	// по часовой стрелке
	{-0.5, -0.5,  0.5}, {-0.5,  0.5,  0.5}, { 0.5,  0.5,  0.5}, { 0.5, -0.5,  0.5}, // front
	{ 0.5, -0.5, -0.5}, { 0.5,  0.5, -0.5}, {-0.5,  0.5, -0.5}, {-0.5, -0.5, -0.5}, // back
	{-0.5, -0.5, -0.5}, {-0.5, -0.5,  0.5}, { 0.5, -0.5,  0.5}, { 0.5, -0.5, -0.5}, // top
	{-0.5,  0.5,  0.5}, {-0.5,  0.5, -0.5}, { 0.5,  0.5, -0.5}, { 0.5,  0.5,  0.5}, // bottom
	{ 0.5, -0.5,  0.5}, { 0.5,  0.5,  0.5}, { 0.5,  0.5, -0.5}, { 0.5, -0.5, -0.5}, // left
	{-0.5, -0.5, -0.5}, {-0.5,  0.5, -0.5}, {-0.5,  0.5,  0.5}, {-0.5, -0.5,  0.5}  // right
};

static const uint32_t __vertexIndex[INDEXCOUNT] = 
{
	0, 3, 2,  2, 1, 0,  // front
	4, 7, 6,  6, 5, 4,  // back
	8,11, 10, 10,9, 8,  // top
	12,15,14, 14,13,12, // bottom
	16,19,18, 18,17,16, // left
	20,23,22, 22,21,20  // right
};

class Cube
{
private:
	float x, y, z;
	float textureCoord[24][2];

	Texture texture;

public:
	Cube(void);
	~Cube(void);

	void SetPos(const vec3 &pos);
	void SetTextureAllSide(const ImageAtlas &atlas, std::string texture);

	template <class TList>
	unsigned int InsertBackIndex(BufferArray<TList> &buffer, unsigned int v)
	{
		static_assert(Lenght<TList>::value == 2, "Unsupported format vertices.");

		// ѕровер€ем есть ли нужные нам типы в списке типов
		static_assert(Find<TList, Vertex>::value != -1, "Unsupported format vertices.");
		static_assert(Find<TList, TextCoord>::value != -1, "Unsupported format vertices.");
		
		// ћы собираемс€ добавить 36 индесков
		buffer.InsertBackIndex(36);
		unsigned int *data = buffer.GetIndexData(buffer.GetSizeIndex() - 36);

		for (unsigned int i = 0; i < 36; i++)
		{
			data[i] = v + __vertexIndex[i];
		}

		return 0;
	};

	template <class TList>
	unsigned int InsertBackVertex(BufferArray<TList> &buffer)
	{
		static_assert(Lenght<TList>::value == 2, "Unsupported format vertices.");

		// ѕровер€ем есть ли нужные нам типы в списке типов
		static_assert(Find<TList, Vertex>::value != -1, "Unsupported format vertices.");
		static_assert(Find<TList, TextCoord>::value != -1, "Unsupported format vertices.");

		// ћы собираемс€ добавить 4 вершины
		buffer.InsertBackVertex(24);
		float *data = buffer.GetVertexData(buffer.GetSizeVertex() - 24);
		
		for(unsigned int i = 0; i < 6; i++)
		{
			VertexAccess<TList, Vertex>(data, i * 4 + 0).x = __vertexPositions[i * 4 + 0][0] + x;
			VertexAccess<TList, Vertex>(data, i * 4 + 0).y = __vertexPositions[i * 4 + 0][1] + y;
			VertexAccess<TList, Vertex>(data, i * 4 + 0).z = __vertexPositions[i * 4 + 0][2] + z;

			VertexAccess<TList, TextCoord>(data, i * 4 + 0).u = textureCoord[i * 4 + 0][0];
			VertexAccess<TList, TextCoord>(data, i * 4 + 0).v = textureCoord[i * 4 + 0][1];

			VertexAccess<TList, Vertex>(data, i * 4 + 1).x = __vertexPositions[i * 4 + 1][0] + x;
			VertexAccess<TList, Vertex>(data, i * 4 + 1).y = __vertexPositions[i * 4 + 1][1] + y;
			VertexAccess<TList, Vertex>(data, i * 4 + 1).z = __vertexPositions[i * 4 + 1][2] + z;

			VertexAccess<TList, TextCoord>(data, i * 4 + 1).u = textureCoord[i * 4 + 1][0];
			VertexAccess<TList, TextCoord>(data, i * 4 + 1).v = textureCoord[i * 4 + 1][1];

			VertexAccess<TList, Vertex>(data, i * 4 + 2).x = __vertexPositions[i * 4 + 2][0] + x;
			VertexAccess<TList, Vertex>(data, i * 4 + 2).y = __vertexPositions[i * 4 + 2][1] + y;
			VertexAccess<TList, Vertex>(data, i * 4 + 2).z = __vertexPositions[i * 4 + 2][2] + z;

			VertexAccess<TList, TextCoord>(data, i * 4 + 2).u = textureCoord[i * 4 + 2][0];
			VertexAccess<TList, TextCoord>(data, i * 4 + 2).v = textureCoord[i * 4 + 2][1];;

			VertexAccess<TList, Vertex>(data, i * 4 + 3).x = __vertexPositions[i * 4 + 3][0] + x;
			VertexAccess<TList, Vertex>(data, i * 4 + 3).y = __vertexPositions[i * 4 + 3][1] + y;
			VertexAccess<TList, Vertex>(data, i * 4 + 3).z = __vertexPositions[i * 4 + 3][2] + z;

			VertexAccess<TList, TextCoord>(data, i * 4 + 3).u = textureCoord[i * 4 + 3][0];
			VertexAccess<TList, TextCoord>(data, i * 4 + 3).v = textureCoord[i * 4 + 3][1];
		}
		
		return 0;
	};

	void SetTexture(Texture tex);
	Texture &GetTexture();

};


#endif // Cube_h__
#include "BufferArray.h"


BufferArrayVTI::BufferArrayVTI(void)
{
}

BufferArrayVTI::BufferArrayVTI( unsigned int sizeVertex, unsigned int sizeTextureCoord, unsigned int sizeIndex )
{
	arrayVertex.reserve(sizeVertex);
	arrayTextureCoord.reserve(sizeTextureCoord);
	arrayIndex.reserve(sizeIndex);
}


BufferArrayVTI::~BufferArrayVTI(void)
{
}

void BufferArrayVTI::AddArray( BufferArrayVTI &bufferArray )
{
	arrayVertex.insert(arrayVertex.end(), bufferArray.arrayVertex.begin(), bufferArray.arrayVertex.end());
	arrayTextureCoord.insert(arrayTextureCoord.end(), bufferArray.arrayTextureCoord.begin(), bufferArray.arrayTextureCoord.end());

	unsigned int vertexCount = arrayVertex.size() / 3;
	for(unsigned int i = 0; i < bufferArray.arrayIndex.size(); i++)
	{
		 bufferArray.arrayIndex[i] += vertexCount;
	}
	arrayIndex.insert(arrayIndex.end(), bufferArray.arrayIndex.begin(), bufferArray.arrayIndex.end());
}

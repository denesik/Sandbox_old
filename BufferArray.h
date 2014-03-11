#ifndef BufferArray_h__
#define BufferArray_h__

#include <vector>
#include "GameMath.h"

typedef std::vector<float>		ArrayVertex;
typedef std::vector<float>		ArrayTextureCoord;
typedef std::vector<float>		ArrayColor;
typedef std::vector<uint32_t>	ArrayIndex;


class BufferArrayVTI
{
public:
	ArrayVertex			arrayVertex;
	ArrayTextureCoord	arrayTextureCoord;
	ArrayIndex			arrayIndex;

public:
	BufferArrayVTI(void);
	BufferArrayVTI(unsigned int sizeVertex, unsigned int sizeTextureCoord, unsigned int sizeIndex);
	~BufferArrayVTI(void);

	void AddArray(BufferArrayVTI &bufferArray);
};


#endif // BufferArray_h__
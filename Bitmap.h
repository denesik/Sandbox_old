#ifndef Bitmap_h__
#define Bitmap_h__

#include <string>

class Bitmap
{
public:
	unsigned char *data;
	unsigned int width;
	unsigned int height;


public:
	Bitmap(void);
	~Bitmap(void);
};


Bitmap *LoadFile(std::string fileName);


#endif // Bitmap_h__
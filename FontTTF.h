#ifndef FontTTF_h__
#define FontTTF_h__

#include <ft2build.h>
#include <freetype.h>
#include <ftglyph.h>
//#include <ftoutln.h>
//#include <fttrigon.h>
#include <ftmodapi.h>

#include "ImageAtlas.h"
#include <string>
#include "TextureManager.h"

class FontTTF
{
private:

	struct FontTexture
	{
		Texture texture;
		unsigned int width;
		unsigned int height;
		int offsetDown;
	};

	FT_Library library;

	FT_Face face;


	ImageAtlas glyphAtlas;
	std::string filename;

	unsigned int size;

public:
	FontTTF( std::string fileName, unsigned int size );
	~FontTTF(void);

	bool Generate();

private:
	bool Init();
	void Finalize();

	void GenerateGlyph(unsigned int gliphNumber, Bitmap &glyphImage, int &glyphOffsetDown);


};


#endif // FontTTF_h__
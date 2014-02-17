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
#include <map>
#include <list>
#include <vector>
#include "Render.h"

class Font
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

	struct GlyphBitmap
	{
		Bitmap* bitmap;
		int offsetDown;
		unsigned int key;

		bool operator < (const GlyphBitmap& second)
		{
			return (bitmap->GetHeight() * bitmap->GetWidth())
				< (second.bitmap->GetHeight() * second.bitmap->GetWidth());
		}
	};

	
	std::list<GlyphBitmap> glyphsBitmapList;

	std::map<unsigned int, FontTexture> glyphsTextureMap;



	ArrayVertex arrayVertex;
	ArrayTextureCoord arrayTextureCoord;
	ArrayIndex arrayIndex;


public:
	Font( std::string configFileName );
	~Font(void);

private:
	bool Init();
	void Finalize();

	bool LoadConfig( std::string configFileName );

	bool GenerateGlyphsList( std::string glyphList );
	bool GenerateGlyph(unsigned int gliphNumber, GlyphBitmap &glyphBitmap);
	bool GenerateOpenglGlyphs();

public:
	ArrayIndex &Print( float x, float y, std::vector<unsigned int> text, Render *render);

};


#endif // FontTTF_h__
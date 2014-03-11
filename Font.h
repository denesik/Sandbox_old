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

struct FontTexture
{
	Texture texture;
	unsigned int width;
	unsigned int height;
	int offsetDown;
};

class Font
{
private:

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


private:
	bool LoadConfig( std::string configFileName );

	bool GenerateGlyphsList( std::string glyphList );
	bool GenerateGlyph(unsigned int gliphNumber, GlyphBitmap &glyphBitmap);
	bool GenerateOpenglGlyphs();

private:
	static Font* instance;

	Font( std::string configFileName );
	Font(const Font& root);
	Font& operator=(const Font&);

public:
	~Font();

	static bool Init( std::string configFileName );
	static void Finalize();
	
	static Font* GetInstance();

	//ArrayIndex &Print( float x, float y, std::vector<unsigned int> text, Render *render);

	FontTexture GetGlyphTexture(unsigned int utf32glyph);

};


#endif // FontTTF_h__
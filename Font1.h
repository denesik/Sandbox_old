#ifndef Font1_h__
#define Font1_h__

#include <ft2build.h>
#include <freetype.h>
#include <ftglyph.h>
//#include <ftoutln.h>
//#include <fttrigon.h>
#include <ftmodapi.h>
#include <string>
#include "GameMath.h"
#include "ImageAtlas.h"


struct FontGlyph
{
	float u1;
	float v1;
	float u2;
	float v2;
	unsigned int width;
	unsigned int height;
	int offsetDown;
};


class Font1
{
private:
	FT_Library library;
	FT_Face face;

	Atlas *glyphAtlas;

	std::string fontName;
	std::string fileName;
	unsigned int size;

	struct FontGlyphTexture
	{
		FontGlyph fontGlyph;
		gm::Rectangle rect;
	};

	std::map<unsigned int, FontGlyphTexture> glyphsTextureMap;

	FontGlyphTexture emptyGlyph;

public:
	Font1(std::string fileName, std::string fontName, unsigned int size, Atlas *atlas);
	~Font1(void);
	Font1(std::string configFileName, Atlas *atlas);

	bool Save(std::string dir = "") const;

	// Генерируем текстурные координаты для глифов
	bool Create();

	// Генерируем символ в кодировке utf-8
	bool CreateGlyph(std::string utf8glyph);

	// получаем символ в кодировке utf-8
	const FontGlyph &GetGlyph(std::string utf8glyph) const;

private:
	bool GenerateEmptyGlyph();

};


#endif // Font1_h__
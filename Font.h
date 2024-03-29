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


class Font
{
private:
	FT_Library library;
	FT_Face face;

	ImageAtlas *glyphAtlas;

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
	Font(std::string fileName, std::string fontName, unsigned int size, ImageAtlas *atlas);
	~Font(void);
	Font(std::string configFileName, ImageAtlas *atlas);

	bool Save(std::string dir = "") const;

	// ���������� ���������� ���������� ��� ������
	bool Create();

	// ���������� ������ � ��������� utf-32
	bool CreateGlyph(unsigned int utf32glyph);

	// �������� ������ � ��������� utf-32
	const FontGlyph &GetGlyph(unsigned int utf32glyph) const;

	bool FindGlyph(unsigned int utf32glyph) const;

	std::string GetName() const;

private:
	bool GenerateEmptyGlyph();

};


#endif // Font1_h__
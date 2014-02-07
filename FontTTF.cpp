#include "FontTTF.h"
#include "Logger.h"
#include "Bitmap.h"


FontTTF::FontTTF( std::string _fileName, unsigned int _size )
{
	glyphAtlas.Create(Bitmap::FORMAT_LUMINANCE_ALPHA, 256, 256);
	filename = _fileName;
	size = _size;
	library = nullptr;
	face = nullptr;
	Init();

	if (FT_New_Face( library, filename.c_str(), 0, &face )) 
	{
		LOG(LOG_ERROR, "FreeType. Файл шрифтов " + filename + " не загружен.");
	}
}


FontTTF::~FontTTF(void)
{
	glyphAtlas.Remove();
	Finalize();
}

bool FontTTF::Init()
{
	if (FT_Init_FreeType( &library )) 
	{
		LOG(LOG_ERROR, "FreeType не инициализирвоан.");
		return false;
	}
	return true;
}

void FontTTF::Finalize()
{
	// delete ogl image atlas
	if(face)
	{
		FT_Done_Face(face);
		face = nullptr;
	}
	if(library)
	{
		FT_Done_FreeType(library);
		library = nullptr;
	}
}

bool FontTTF::Generate()
{
	FontTexture fontTexture;

	Bitmap *atlasImage = glyphAtlas.GetAtlas();

	for(unsigned int i = 0; i < 256; i++)
	{
		Bitmap glyphImage;
		iRect rect;

		GenerateGlyph(i, glyphImage, fontTexture.offsetDown);
		glyphAtlas.InsertImage(&glyphImage, rect);

		fontTexture.width = glyphImage.GetWidth();
		fontTexture.height = glyphImage.GetHeight();
		GenerateTextCoord(atlasImage, &rect, fontTexture.texture);

		glyphImage.Free();
	}

	fontTexture.texture.textureId = GenerateOpenglBitmap(*atlasImage, false);

//	glyphAtlas.GetAtlas()->Save("atlas.png");

	return true;
}

void FontTTF::GenerateGlyph( unsigned int glyphNumber, Bitmap &glyphImage, int &glyphOffsetDown)
{
	// Сделаем высоту немного больше, что бы оставить место между линиями.
	float fsize = float(size) / 0.63f;		

	// По некоторым причинам FreeType измеряет размер шрифта в терминах 1/64 пикселя.
	// Таким образом, для того чтобы сделать шрифт выстой size пикселей, мы запрашиваем размер size*64.
	// (size << 6 тоже самое что и size*64)
	FT_Set_Char_Size( face, size << 6, size << 6, 96, 96);


	if(FT_Load_Glyph( face, FT_Get_Char_Index( face, glyphNumber ), FT_LOAD_DEFAULT ))
	{
		LOG(LOG_ERROR, "FreeType. Невозможно загрузить глиф.");
	}

	FT_Glyph glyph;
	if(FT_Get_Glyph( face->glyph, &glyph ))
	{
		LOG(LOG_ERROR, "FreeType. Невозможно загрузить глиф.");
	}

	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	FT_Bitmap &bitmap=bitmap_glyph->bitmap;


	glyphImage.Generate(Bitmap::FORMAT_LUMINANCE_ALPHA, bitmap.width, bitmap.rows, 0x00000000);
	unsigned int channelCount = GetChannelCount(Bitmap::FORMAT_LUMINANCE_ALPHA);

	byte *glyphImageData = glyphImage.GetData();
	
	/*
	for(int i = 0; i < bitmap.width * bitmap.rows; i++)
	{
		glyphImageData[i * channelCount + channelCount - 1] = bitmap.buffer[i];
	}
	*/

	for(int j = 0; j < bitmap.rows; j++) 
	{
		for(int i=0; i < bitmap.width; i++)
		{
 			glyphImageData[( bitmap.width * j + i ) * channelCount + channelCount - 1] 
 				= bitmap.buffer[bitmap.width * ( bitmap.rows - j - 1) + i];
		}
	}
	
	glyphOffsetDown = bitmap_glyph->top - bitmap.rows;

}

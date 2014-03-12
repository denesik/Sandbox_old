#include "GraphicText.h"
#include "Font.h"
#include "Rectangle.h"
#include <utf8.h>


GraphicText::GraphicText(void) : buffer(false, true, false)
{
	x = 0;
	y = 0;
	z = 0;
	constraintWidth = 0;
	constraintHeight = 0;
}


GraphicText::~GraphicText(void)
{
}

void GraphicText::SetText( std::string text )
{
	utf32text.clear();
	utf8::utf8to32(text.begin(), text.end(), std::back_inserter(utf32text));
	buffer.Clear();
	buffer.DeleteVideoBuffer();
	CreateBuffer();
}

void GraphicText::CreateBuffer()
{
	Rectangle geometryRectangle;
	FontTexture fontTexture;
	float glyphX = x;
	float glyphY = y;
	float stringHeight = 22.0f;

	for(unsigned int i = 0; i < utf32text.size(); i++)
	{
		// if ������, ������� ������, ���������
		fontTexture = Font::GetInstance()->GetGlyphTexture(utf32text[i]);

		geometryRectangle.SetPos(vec3(glyphX, glyphY + stringHeight - fontTexture.height - fontTexture.offsetDown, z));
		geometryRectangle.SetSize((float)fontTexture.width, (float)fontTexture.height);
		geometryRectangle.SetTexture(fontTexture.texture);

		glyphX += fontTexture.width;

		buffer.PushBack(geometryRectangle.GetBufferArray());
	}

	buffer.CreateVideoBuffer();

}

void GraphicText::SetPos( const vec3 &pos )
{
	x = pos[0];
	y = pos[1];
	z = pos[2];
}

void GraphicText::Draw()
{
	glBindTexture(GL_TEXTURE_2D, Font::GetInstance()->GetGlyphTexture(97).texture.textureId);
	buffer.Draw();
}


/*
ArrayIndex &Font::Print( float x, float y, std::vector<unsigned int> text, Render *render)
{
	Rectangle geometryRectangle;
	FontTexture fontTexture = glyphsTextureMap[97];

	unsigned int textLenght = text.size();

	float glyphX = x;
	float glyphY = y;
	float stringHeight = 22.0f;

	for(unsigned int i = 0; i < textLenght; i++)
	{
		fontTexture = glyphsTextureMap[text[i]];

		geometryRectangle.SetPos(vec3(glyphX, glyphY + stringHeight - fontTexture.height - fontTexture.offsetDown, -1));
		geometryRectangle.SetSize((float)fontTexture.width, (float)fontTexture.height);
		geometryRectangle.SetTexture(fontTexture.texture);

		glyphX += fontTexture.width;

		buffer.AddArray(geometryRectangle.GetBufferArrayVTI());
	}

	render->CreateBufferArrayVTI(buffer);
	glBindTexture(GL_TEXTURE_2D, fontTexture.texture.textureId);

	return buffer.arrayIndex;

}
*/
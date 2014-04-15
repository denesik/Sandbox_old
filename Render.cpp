#include "Render.h"
#include "Logger.h"

#define GLM_FORCE_RADIANS
#include <gtc/matrix_transform.hpp>
#include <vector>


std::shared_ptr<Texture> CreateTexture( const Bitmap &bitmap, bool smoothing )
{

	unsigned int glBitmap = 0;

	Bitmap::PixelFormat format = bitmap.GetFormat();
	unsigned int colorType = GL_RGB;
	switch (format)
	{
	case Bitmap::FORMAT_LUMINANCE:
		{
			colorType = GL_LUMINANCE;
			break;
		}

	case Bitmap::FORMAT_LUMINANCE_ALPHA:
		{
			colorType = GL_LUMINANCE_ALPHA;
			break;
		}

	case Bitmap::FORMAT_RGB:
		{
			colorType = GL_RGB;
			break;
		}

	case Bitmap::FORMAT_RGBA:
		{
			colorType = GL_RGBA;
			break;
		}

	default:
		{
			//LOG(LOG_WARNING, "Generate GLBitmap. Не поддерживаемый тип цвета.");
			return nullptr;
		}
	}


	glGenTextures(1, &glBitmap);
	glBindTexture(GL_TEXTURE_2D, glBitmap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smoothing ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smoothing ? GL_LINEAR : GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // выравнивание

	glTexImage2D(GL_TEXTURE_2D, 0, colorType, bitmap.GetWidth(), bitmap.GetHeight(), 0, colorType, GL_UNSIGNED_BYTE, bitmap.GetData());
	OPENGL_CHECK_ERRORS();
	
	return std::make_shared<Texture>(glBitmap, bitmap.GetSize());
}


Render::Render(void)
{
	width = 0;
	height = 0;
	orthoProjection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, 0.1f, 100.0f);
}


Render::~Render(void)
{
}

bool Render::Init()
{

	glewExperimental=true; // Needed in core profile

	if (glewInit() != GLEW_OK) 
	{
		LOG_ERROR("GLEW не инициализирован");
		return false;
	}
	glGetError();

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);



	OPENGL_CHECK_ERRORS();

	return true;
}




void Render::SetWindowSize( unsigned int _width, unsigned int _height )
{
	width = _width;
	height = _height;
}

glm::mat4 Render::GetOrthoProjection()
{
	orthoProjection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, 0.1f, 100.0f);
	return orthoProjection;
}
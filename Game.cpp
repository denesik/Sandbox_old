#include "Game.h"
#include "Logger.h"
#include "Bitmap.h"


static unsigned int GenerateOpenglBitmap(Bitmap &bitmap, bool smoothing)
{
	unsigned int glBitmap = 0;
	glGenTextures(1, &glBitmap);
	glBindTexture(GL_TEXTURE_2D, glBitmap);

	GLint param = GL_LINEAR;
	if(!smoothing) param = GL_NEAREST;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);

	unsigned int format = bitmap.GetFormat();
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
			LOG(LOG_WARNING, "Generate GLBitmap. Не поддерживаемый тип цвета.");
			break;
		}
	}

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, colorType, bitmap.GetWidth(), bitmap.GetHeight(), 0, colorType, GL_UNSIGNED_BYTE, bitmap.GetData());

	return glBitmap;
}


void errorCallbackGLFW3(int error, const char* description)
{
	
}

Game::Game(void)
{
	Running = true;

	title = "Game";
	width = 800;
	height = 600;
	fullscreen = false;

}


Game::~Game(void)
{

}

bool Game::Initialize()
{
	
	glfwSetErrorCallback(errorCallbackGLFW3);

	if (!glfwInit())
	{
		LOG(LOG_ERROR, "Ошибка инициализации GLFW.");
		return false;
	}
	
	GLFWmonitor *monitor = NULL;

	if(fullscreen)
	{
		monitor = glfwGetPrimaryMonitor();
	}

	window = glfwCreateWindow(width, height, title, monitor, NULL);
	if (!window)
	{
		glfwTerminate();
		LOG(LOG_ERROR, "Ошибка создания окна GLFW.");
		return false;
	}
	glfwMakeContextCurrent(window);
	
	/*

	EventManager::Init();

	render->Initialize(title, width, height, fullscreen);
	*/

	Bitmap *b = new Bitmap();
	

	//b->Load("img.png");
	
	unsigned int channel = 4;

	byte *data = new byte[32*32*channel];

	for(unsigned int i = 0; i < 32*32; i++)
	{
		if(i % 2 == 0)
		{
			data[i*channel] = 100;
			data[i*channel + 1] = 50;
			data[i*channel + 2] = 100;
		}
		else
		{
			data[i*channel] = 200;
			data[i*channel + 1] = 100;
			data[i*channel + 2] = 200;
		}

		data[i*channel + channel - 1] = 128;
	}

	b->Change(Bitmap::FORMAT_RGBA, 32, 32, data);
	
	b->ConvertFormat(Bitmap::FORMAT_LUMINANCE_ALPHA);

	b->Save("img.png");
	texture = 0;
	texture = GenerateOpenglBitmap(*b, false);

	b->Free();

	return true;
}

void Game::LoadContent()
{
}

int Game::Run()
{
	
	if(!Initialize()) 
	{
		LOG(LOG_ERROR, "Инициализация завершилась с ошибками.");
		return -1;
	}

	LoadContent();



	while(Running && !glfwWindowShouldClose(window)) 
	{
		


		Update();
		Draw();
		
		float ratio = width / (float) height;
		glViewport(0, 0, width, height);

// 		glMatrixMode(GL_PROJECTION);
// 		glLoadIdentity();
//		glOrtho(-ratio, ratio, -1.f, 1.f, 10.f, -10.f);
//		
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity( );

		gluPerspective( 45.0f, ratio, 0.1f, 100.0f );

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glShadeModel(GL_SMOOTH);							
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				
		glClearDepth(1.0f);									
		glEnable(GL_DEPTH_TEST);							
		glDepthFunc(GL_LEQUAL);								
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		glEnable(GL_TEXTURE_2D);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glTranslatef(0.0f, 0.0f, -7.0f);
		glRotatef((float) glfwGetTime() * 50.f, 0.f, 1.f, 1.f);
	
		glBindTexture(GL_TEXTURE_2D, texture);

		glBegin(GL_QUADS);

			// Передняя грань
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Низ лево
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Низ право
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Верх право
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Верх лево

			// Задняя грань
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Низ право
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Верх право
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Верх лево
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Низ лево

			// Верхняя грань
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Верх лево
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Низ лево
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Низ право
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Верх право

			// Нижняя грань
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Верх право
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Верх лево
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Низ лево
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Низ право

			// Правая грань
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Низ право
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Верх право
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Верх лево
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Низ лево

			// Левая грань
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Низ лево
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Низ право
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Верх право
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Верх лево

		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	UnloadContent();

	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}

void Game::Update()
{

}

void Game::Draw()
{

}

void Game::UnloadContent()
{
	
}

void Game::ResizeWindow( unsigned int _width, unsigned int _height )
{
	/*
	width = _width;
	height = _height;
	render->ResizeWindow(width, height);
	*/
}

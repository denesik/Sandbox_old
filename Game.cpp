#include "Game.h"
#include "Logger.h"
#include "Bitmap.h"
#include "TextureManager.h"
#include "ImageAtlas.h"
#include "Font.h"


#include <vector>
#include "Cube.h"
#include "Camera.h"
#define GLM_FORCE_RADIANS
#include <gtc/matrix_transform.hpp>
#include "Keyboard.h"
#include "Mouse.h"
#include "Rectangle.h"

#include "Logger.h"
#include "utf8.h"
#include "Sector.h"
#include "FPSCounter.h"

#include <sstream>
#include "GraphicText.h"

GLuint LoadShaders(std::string vertex_file_path,std::string fragment_file_path)
{

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	//LOG(LOG_INFO, "Compiling shader: \"" + vertex_file_path + "\".");
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	//LOG(LOG_INFO, &VertexShaderErrorMessage[0]);

	// Compile Fragment Shader
	//LOG(LOG_INFO, "Compiling shader: \"" + fragment_file_path + "\".");
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	//LOG(LOG_INFO, &FragmentShaderErrorMessage[0]);

	// Link the program
	//LOG(LOG_INFO, "Linking program");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	//LOG(LOG_INFO, &ProgramErrorMessage[0]);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void KeyCallbackGLFW3(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	Keyboard::SetKey(key, scancode, action, mods);
}

void CursorPosCallbackGLFW3(GLFWwindow *window, double xpos, double ypos)
{
	Mouse::SetCursorPos(xpos, ypos);
}

void CursorClientAreaCallbackGLFW3(GLFWwindow *window, int entered)
{
	Mouse::CursorClientArea(entered);
}

void WindowFocusCallbackGLFW3(GLFWwindow *window, int focused)
{
	Mouse::WindowFocus(focused);
}

void errorCallbackGLFW3(int error, const char* description)
{
	LOG_ERROR("GLFW. %s", description);
}

Game::Game(void)
{
	Running = true;

	title = "Game";
	width = 800;
	height = 600;
	fullscreen = false;
	render = nullptr;

}


Game::~Game(void)
{

}

bool Game::Initialize()
{
	
	glfwSetErrorCallback(errorCallbackGLFW3);

	if (!glfwInit())
	{
		LOG_ERROR("GLFW не инициализирован.");
		return false;
	}
	
//	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	GLFWmonitor *monitor = NULL;

	if(fullscreen)
	{
		monitor = glfwGetPrimaryMonitor();
	}

	window = glfwCreateWindow(width, height, title, monitor, NULL);
	if (!window)
	{
		glfwTerminate();
		LOG_ERROR("Не удалось создать окно.");
		return false;
	}
	// Создаем контекст
	glfwMakeContextCurrent(window);
	
	LOG_INFO("Окно успешно создано.");

//============ "Окно создано" ========================

	Keyboard::Init();
	glfwSetKeyCallback(window, KeyCallbackGLFW3);

	Mouse::Init(window);
	Mouse::SetWindowSize(width, height);
	//	Mouse::SetFixedPosState(true);
	glfwSetCursorPosCallback(window, CursorPosCallbackGLFW3);
	glfwSetCursorEnterCallback(window, CursorClientAreaCallbackGLFW3);	
	glfwSetWindowFocusCallback(window, WindowFocusCallbackGLFW3);

	// Отключаем вертикальную синхронизацию
	glfwSwapInterval(0);


	render = new Render;
	if(!render->Init())
	{
		LOG_ERROR("Рендер не инициализирован.");

		delete render;
		render = nullptr;
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}
	// настройки рендера.
	render->SetWindowSize(width, height);

	LOG_INFO("Рендер инициализирован.");

	//*******************************

	if(!Font::Init())
	{
		LOG_ERROR("Ошибка инициализации шрифтов.");
		delete Font::GetInstance();
		delete render;
		render = nullptr;
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}
	LOG_INFO("Шрифты инициализированы.");


	Bitmap *b = new Bitmap();

	b->Load("img.png");

	texture = 0;
	texture = GenerateOpenglBitmap(*b, true, true);

	b->Free();

	glActiveTexture(GL_TEXTURE1);

	return true;
}

void Game::LoadContent()
{
	if(Font::GetInstance()->Create("font.json"))
	{
		LOG_INFO("Шрифты Загружены.");
	}
}

template< typename T >
std::string ToString( const T& val )
{
	std::stringstream iss;
	iss << val;
	return iss.str();
}

int Game::Run()
{

	if(!Initialize()) 
	{
		LOG_INFO("Инициализация завершилась с ошибками.");
		return -1;
	}
	LOG_INFO("Инициализация прошла успешно.");

	LoadContent();

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "shaders/t2.vs", "shaders/t2.fs" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	Camera camera;
	camera.SetWindowSize(width, height);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 MVP = camera.CalculateMatrix() * model;

	LOG_INFO("1");

 	Rectangle geometryRectangle;
 	geometryRectangle.SetPos(vec3(0, 50, -1));
 	geometryRectangle.SetSize(100, 100);
 	Texture tex;
 	tex.u1 = 0.0f;
 	tex.v1 = 0.0f;
 	tex.u2 = 1.0f;
 	tex.v2 = 1.0f;
 	geometryRectangle.SetTexture(tex);


// 	BufferArrayRoot ba(false, true, false);
// 	ba.PushBack(geometryRectangle.GetBufferArray());
// 	ba.CreateVideoBuffer();


//	Sector map;
//	map.CreateGeometry();
	
	BufferArray<VTC> buffer;
	buffer.CreateVideoBuffer();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// делаем активным текстурный юнит 0
	glActiveTexture(GL_TEXTURE1);
	// назначаем текстуру на активный текстурный юнит
	glBindTexture(GL_TEXTURE_2D, 1);

	GLint textureLocation = -1;
	textureLocation = glGetUniformLocation(programID, "colorTexture");


	float const speed = 0.05f;

	FPSCounter fps;
// 
// 	GraphicText fpsText;
// 	fpsText.SetPos(vec3(10, 10, -1));
// 
// 	GraphicText testText;
// 	testText.SetPos(vec3(500, 500, -1));
// 	testText.SetText("tessst1");


	while(Running && !glfwWindowShouldClose(window)) 
	{
		
		fps.Update();
		auto a = ToString(fps.GetCount()) + "=-+";
	//	fpsText.SetText(a);
		glfwSetWindowTitle(window, a.c_str());

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		if(Keyboard::isKeyDown(GLFW_KEY_W))
		{
			camera.MoveZ(speed);
		}

		if(Keyboard::isKeyDown(GLFW_KEY_S))
		{
			camera.MoveZ(-speed);
		}

		if(Keyboard::isKeyDown(GLFW_KEY_SPACE))
		{
			camera.MoveY(speed);
		}

		if(Keyboard::isKeyDown(GLFW_KEY_LEFT_SHIFT))
		{
			camera.MoveY(-speed);
		}

		if(Keyboard::isKeyDown(GLFW_KEY_D))
		{
			camera.MoveX(speed);
		}

		if(Keyboard::isKeyDown(GLFW_KEY_A))
		{
			camera.MoveX(-speed);
		}

		float dx = float(Mouse::IsMoveCursorX());
		if( dx != 0)
		{
			camera.RotateX( dx );
		}

		float dy = float(Mouse::IsMoveCursorY());
		if( dy != 0)
		{
			camera.RotateY( dy );
		}
		

		MVP = camera.CalculateMatrix() * model;
		// Use our shader
		glUseProgram(programID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniform1i(textureLocation, 1);

		glBindTexture(GL_TEXTURE_2D, 1);
//		map.Draw();

		MVP = render->GetOrthoProjection();

		// Use our shader
		glUseProgram(programID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniform1i(textureLocation, 1);

	/*	ba.Draw();
		fpsText.Draw();
		
 		for(unsigned int i = 0; i < 100; i++)
 		{
 			ba.Reset();
 			ba.PushBack(geometryRectangle.GetBufferArray());
 			ba.CreateVideoBuffer();
			ba.Draw();
 		}
*/
/*		for(unsigned int i = 0; i < 100; i++)
		{
			testText.SetText("ttttt");
			testText.Draw();
		}
	*/	
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteProgram(programID);

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

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
	LOG(LOG_INFO, "Compiling shader: \"" + vertex_file_path + "\".");
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	LOG(LOG_INFO, &VertexShaderErrorMessage[0]);

	// Compile Fragment Shader
	LOG(LOG_INFO, "Compiling shader: \"" + fragment_file_path + "\".");
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	LOG(LOG_INFO, &FragmentShaderErrorMessage[0]);

	// Link the program
	LOG(LOG_INFO, "Linking program");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	LOG(LOG_INFO, &ProgramErrorMessage[0]);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
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
		LOG(LOG_ERROR, "������ ������������� GLFW.");
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
		LOG(LOG_ERROR, "������ �������� ���� GLFW.");
		return false;
	}
	glfwMakeContextCurrent(window);
	

	render = new Render;
	render->Init();


	//*******************************

	Bitmap *b = new Bitmap();

	b->Load("img.png");

	texture = 0;
	texture = GenerateOpenglBitmap(*b, true);

	b->Free();

	Font font("font.json");

	return true;
}

void Game::LoadContent()
{
}

int Game::Run()
{
	
	if(!Initialize()) 
	{
		LOG(LOG_ERROR, "������������� ����������� � ��������.");
		return -1;
	}

	LoadContent();

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	glEnable(GL_TEXTURE_2D);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "shaders/t2.vs", "shaders/t2.fs" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	Camera camera;
	camera.SetWindowSize(width, height);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 MVP = camera.CalculateMatrix() * model;


	Cube cube;

	GLuint VertexArrayID;
	VertexArrayID = render->CreateVertexArrayObject();

	GLuint vertexbuffer;
	vertexbuffer = render->CreateBufferVertex(cube.GetVertexPosition( vec3(0.0f, 0.0f, 0.0f) ));

	GLuint texturebuffer;
	texturebuffer = render->CreateBufferTextCoord(cube.GetTextureCoord());

	GLuint indexbuffer;
	indexbuffer = render->CreateBufferIndex(cube.GetVertexIndex());

	size_t cubeIndices = cube.GetVertexIndex().sizeElement * cube.GetVertexIndex().lenght;

	// ������ �������� ���������� ���� 0
	glActiveTexture(GL_TEXTURE1);
	// ��������� �������� �� �������� ���������� ����
	glBindTexture(GL_TEXTURE_2D, texture);

	GLint textureLocation = -1;
	textureLocation = glGetUniformLocation(programID, "colorTexture");

	while(Running && !glfwWindowShouldClose(window)) 
	{
		
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniform1i(textureLocation, 1);

		render->UseVertexArrayObject(VertexArrayID);
		glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_INT, NULL);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	// Cleanup VBO and shader
	render->DeleteBufferVertex(vertexbuffer);
//	render->DeleteBufferColor(colorbuffer);

	glDeleteProgram(programID);
	render->DeleteVertexArrayObject(VertexArrayID);

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

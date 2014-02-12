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



// координаты вершин куба
const float cubePositions[24][3] = 
{
	{0, 1, 1}, {1, 1, 1}, {1, 0, 1}, {0, 0, 1}, // front
	{1, 1, 0}, {0, 1, 0}, {0, 0, 0}, {1, 0, 0}, // back
	{0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1}, // top
	{1, 0, 0}, {0, 0, 0}, {0, 0, 1}, {1, 0, 1}, // bottom
	{0, 1, 0}, {0, 1, 1}, {0, 0, 1}, {0, 0, 0}, // left
	{1, 1, 1}, {1, 1, 0}, {1, 0, 0}, {1, 0, 1}  // right
};

// текстурные координаты куба
const float cubeTexcoords[24][2] = 
{
	{0.0f,1.0f}, {1.0f,1.0f}, {1.0f,0.0f}, {0.0f,0.0f}, // front
	{0.0f,1.0f}, {1.0f,1.0f}, {1.0f,0.0f}, {0.0f,0.0f}, // back
	{0.0f,1.0f}, {1.0f,1.0f}, {1.0f,0.0f}, {0.0f,0.0f}, // top
	{0.0f,1.0f}, {1.0f,1.0f}, {1.0f,0.0f}, {0.0f,0.0f}, // bottom
	{0.0f,1.0f}, {1.0f,1.0f}, {1.0f,0.0f}, {0.0f,0.0f}, // left
	{0.0f,1.0f}, {1.0f,1.0f}, {1.0f,0.0f}, {0.0f,0.0f}  // right
};

// индексы вершин куба в порядке поротив часовой стрелки
const uint32_t cubeIndices[36] = 
{
	0, 3, 1,  1, 3, 2, // front
	4, 7, 5,  5, 7, 6, // back
	8,11, 9,  9,11,10, // top
	12,15,13, 13,15,14, // bottom
	16,19,17, 17,19,18, // left
	20,23,21, 21,23,22  // right
};


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
		LOG(LOG_ERROR, "Ошибка инициализации GLFW.");
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
		LOG(LOG_ERROR, "Ошибка создания окна GLFW.");
		return false;
	}
	glfwMakeContextCurrent(window);
	

	render = new Render;
	render->Init();


	//*******************************

	Bitmap *b = new Bitmap();

	b->Load("img.png");

	texture = 0;
	texture = GenerateOpenglBitmap(*b, false);

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
		LOG(LOG_ERROR, "Инициализация завершилась с ошибками.");
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

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around


	GLuint VertexArrayID;
	VertexArrayID = render->CreateVertexArray();

	GLuint vertexbuffer;
	vertexbuffer = render->CreateBufferVertex(sizeof(cubePositions), cubePositions);

	GLuint texturebuffer;
	texturebuffer = render->CreateBufferTextCoord(sizeof(cubeTexcoords), cubeTexcoords);

	GLuint indexbuffer;
	indexbuffer = render->CreateBufferIndex(sizeof(cubeIndices), cubeIndices);

	// делаем активным текстурный юнит 0
	glActiveTexture(GL_TEXTURE1);
	// назначаем текстуру на активный текстурный юнит
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

		render->UseVertexArray(VertexArrayID);
		glDrawElements(GL_TRIANGLES, sizeof(cubeIndices), GL_UNSIGNED_INT, NULL);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	// Cleanup VBO and shader
	render->DeleteBufferVertex(vertexbuffer);
//	render->DeleteBufferColor(colorbuffer);

	glDeleteProgram(programID);
	render->DeleteVertexArray(VertexArrayID);

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

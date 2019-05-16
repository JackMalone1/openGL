#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/// <summary>
/// 
/// </summary>
static void GLClearError()
{

	while (glGetError() != GL_NO_ERROR)
	{

	}
}

/// <summary>
/// 
/// </summary>
/// <param name="function"></param>
/// <param name="file"></param>
/// <param name="line"></param>
/// <returns></returns>
static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[Open GL error] (" << error << ")" << function
			<< " " << file << " line:" << line << std::endl;
		return false;
	}
	return true;
}

/// <summary>
/// 
/// </summary>
struct programShaderSource
{
	std::string vertexSource;
	std::string fragmantSource;
};

/// <summary>
/// 
/// </summary>
/// <param name="filepath"></param>
/// <returns></returns>
static programShaderSource parseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType {
		None = -1,
		Vertex = 0,
		Fragment = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::None;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::Vertex;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::Fragment;
			}
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}

	return { ss[0].str(),ss[1].str() };
}

/// <summary>
/// 
/// </summary>
/// <param name="source"></param>
/// <param name="type"></param>
/// <returns></returns>
static unsigned int compileShader(const std::string& source,
	unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	//error handling if it was unable to succesfully create the shader and tells us what type of shader wasn't created
	//deletes the sahder if it wasn't created properly so we dont get any errors by trying to use a shader that didnt work
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		char* message = (char*)alloca(length * sizeof(char));

		GLCall(glGetShaderInfoLog(id, length, &length, message));

		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
			"shader!" << std::endl;
		std::cout << message << std::endl;

		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

/// <summary>
/// 
/// </summary>
/// <param name="vertexShader"></param>
/// <param name="fragmentShader"></param>
/// <returns></returns>
static unsigned int createShader(const std::string& vertexShader,
	const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fs = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

/// <summary>
/// Recieves and handles key events within the active GLFW window
/// </summary>
/// <param name="t_window">ID of the window</param>
/// <param name="t_key">event keycode</param>
/// <param name="t_scanCode">system specific keycode</param>
/// <param name="t_action">action performed (press/release/etc.)</param>
/// <param name="t_mods">modifiers on the key (shift/ctrl/etc.)</param>
void processInput(GLFWwindow* t_window, int t_key, int t_scanCode, int t_action, int t_mods)
{
	std::string eventType = "";

	switch (t_action)
	{
	case 0:
		eventType = "release";
		break;
	case 1:
		eventType = "press";
		break;
	case 2:
		eventType = "hold";
		break;
	default:
		break;
	}

	std::cout << t_key << ", " << eventType << std::endl;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		return -1;
	}


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "error" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	// vertex positions for our rectangle
	float positions[] = {
		-0.5f,-0.5f, // bottom left (X, Y)
		0.5f,-0.5f, // bottom right (X, Y)
		0.5f,0.5f, // top right (X, Y)
		-0.5f,0.5f, // top left (X, Y)
	};

	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};

	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

	unsigned int indexedBufferObject;
	GLCall(glGenBuffers(1, &indexedBufferObject));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexedBufferObject));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	programShaderSource source = parseShader("res/shaders/Basic.shader");

	unsigned int shader = createShader(source.vertexSource, source.fragmantSource);
	GLCall(glUseProgram(shader));

	/* SET UP KEYBOARD INPUT CALLBACK */

	glfwSetKeyCallback(window, processInput);

	// ********************************

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	GLCall(glDeleteProgram(shader));

	glfwTerminate();
	return 0;
}
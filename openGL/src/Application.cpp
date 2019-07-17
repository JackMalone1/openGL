#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "VertexArray.h"
struct shaderProgramSource
{
	std::string vertexSource;
	std::string fragmantSource;
};

static shaderProgramSource parseShader(const std::string& filepath)
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
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment" ) <<
			"shader!" << std::endl;
		std::cout << message << std::endl;

		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

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

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);//sets a fram rate limit

	if (glewInit() != GLEW_OK)
	{
		std::cout << "error" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		float positions[] = {
			-0.5f,-0.5f,
			0.5f,-0.5f,
			0.5f,0.5f,
			-0.5f,0.5f,
		};

		unsigned int indices[] = {
			0,1,2,
			2,3,0
		};

		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));
		

		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);


		IndexBuffer indexedBufferObject(indices, 6);

		shaderProgramSource source = parseShader("res/shaders/Basic.shader");

		unsigned int shader = createShader(source.vertexSource, source.fragmantSource);
		GLCall(glUseProgram(shader));

		GLCall(int location = glGetUniformLocation(shader, "u_Colour"));//gets the location of the uniform so it can be set
		ASSERT(location != -1);//if it wasn't able to find the location of the uniform or not
		GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));//sets the uniform to the draw has to be done every draw
		
		va.Unbind();
		GLCall(glUseProgram(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(GL_ELEMENT_ARRAY_BUFFER, 0);

		float r = 0.0f;
		float g = 0.3f;
		float b = 0.8f;

		float increment = 0.05f;
		float incrementG = 0.05f;
		float incrementB = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			GLCall(glUseProgram(shader));
			GLCall(glUniform4f(location, r, g, b, 1.0f));//sets the uniform to the draw has to be done every draw

			va.Bind();
			indexedBufferObject.bind();

			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			if (r > 1.0f)
			{
				increment = -0.05f;
			}
			else if (r < 0.0f)
			{
				increment = 0.05f;
			}

			if (g > 1.0f)
			{
				incrementG = -0.05f;
			}
			else if (g < 0.0f)
			{
				incrementG = 0.05f;
			}

			if (b > 1.0f)
			{
				incrementB = -0.05f;
			}
			else if (b < 0.0f)
			{
				incrementB = 0.05f;
			}
			r += increment;
			g += incrementG;
			b += incrementB;
			/* Poll for and process events */
			glfwPollEvents();
		}

		GLCall(glDeleteProgram(shader));
	}
	glfwTerminate();
	return 0;
}
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

#include "Shader.h"

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

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Colour", 0.2f, 0.3f, 0.8f, 1.0f);

		va.Unbind();
		shader.Unbind();
		vb.unBind();
		indexedBufferObject.unBind();

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

			shader.Bind();
			shader.SetUniform4f("u_Colour", r, g, b, 1.0f);
			

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

	}
	glfwTerminate();
	return 0;
}
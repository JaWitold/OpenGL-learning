

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK) {
		return -2;
	}

	{
		constexpr float positions[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f,
		};

		const unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		//Generate vertex array object cause it is mandatory to be initialized in CORE profile

		VertexArray vertex_array;
		VertexBuffer vertex_buffer(positions, sizeof(float) * 4 * 2);

		VertexBufferLayout layout;
		layout.Push<float>(2);
		vertex_array.AddBuffer(vertex_buffer, layout);
		
		IndexBuffer index_buffer(indices, 6);

		
		Shader shader("res/shaders/Basic.shader");

		shader.Unbind();
		vertex_buffer.Unbind();
		vertex_array.Unbind();
		index_buffer.Unbind();

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.01f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			renderer.Clear();
			renderer.Draw(vertex_array, index_buffer, shader);

			if (r > 1.0f || r < 0.0f)
				increment = -increment;
			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}
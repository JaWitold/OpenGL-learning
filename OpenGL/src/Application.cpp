

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(960, 540, "Hello World", nullptr, nullptr);
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
			100.0f, 100.0f, 0.0f, 0.0f,
			200.0f, 100.0f, 1.0f, 0.0f,
			200.0f,  200.0f, 1.0f, 1.0f,
			100.0f,  200.0f, 0.0f, 1.0f
		};

		const unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		//Generate vertex array object cause it is mandatory to be initialized in CORE profile

		VertexArray vertex_array;
		const VertexBuffer vertex_buffer(positions, sizeof(float) * 4 * 4);

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		vertex_array.AddBuffer(vertex_buffer, layout);

		const IndexBuffer index_buffer(indices, 6);

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		const glm::mat4 proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 100.0f);
		const glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
		
		Shader shader("res/shaders/Texture.shader");
		shader.Bind();
		shader.SetUniform1i("u_Texture", 0);

		const Texture texture("res/textures/ChernoLogo.png");
		texture.Bind();

		shader.Unbind();
		vertex_buffer.Unbind();
		vertex_array.Unbind();
		index_buffer.Unbind();

		const Renderer renderer;

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		float r = 0.0f;
		float increment = 0.01f;
		float x = 0.0f, y = 0.0f;
		glm::vec3 translation(200, 200, 0);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();

			ImGui_ImplGlfwGL3_NewFrame();

			const glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
			const glm::mat4 mvp = proj * view * model;
			/* Render here */
			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			shader.SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(vertex_array, index_buffer, shader);

			if (r > 1.0f || r < 0.0f)
				increment = -increment;
			r += increment;

			{
				ImGui::SliderFloat3("Translation x", &translation.x, 0.0f, width);        

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
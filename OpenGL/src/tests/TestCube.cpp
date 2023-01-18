#include "TestCube.h"

#include "Renderer.h"
#include "Texture.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace test
{
	TestCube::TestCube() :
		m_TranslationA(280.0f, 270.0f, 0.0f),
		m_TranslationB(0.0f, 0.0f, 0.0f)
	{
		float positions[] = {
			//front
			-50.0f, -50.0f, -50.0f, 0.0f, 0.0f,
			 50.0f, -50.0f, -50.0f, 1.0f, 0.0f,
			 50.0f,  50.0f, -50.0f, 1.0f, 1.0f,
			-50.0f,  50.0f, -50.0f, 0.0f, 1.0f,

			//back
			 50.0f, -50.0f, 50.0f, 0.0f, 0.0f,
			-50.0f, -50.0f, 50.0f, 1.0f, 0.0f,
			-50.0f,  50.0f, 50.0f, 1.0f, 1.0f,
			 50.0f,  50.0f, 50.0f, 0.0f, 1.0f,

			//left
			-50.0f, -50.0f,  50.0f, 0.0f, 0.0f,
			-50.0f, -50.0f, -50.0f, 1.0f, 0.0f,
			-50.0f,  50.0f, -50.0f, 1.0f, 1.0f,
			-50.0f,  50.0f,  50.0f, 0.0f, 1.0f,

			//right
			50.0f, -50.0f, -50.0f, 0.0f, 0.0f,
			50.0f, -50.0f,  50.0f, 1.0f, 0.0f,
			50.0f,  50.0f,  50.0f, 1.0f, 1.0f,
			50.0f,  50.0f, -50.0f, 0.0f, 1.0f,

			//top
			-50.0f, 50.0f, -50.0f, 0.0f, 0.0f,
			 50.0f, 50.0f, -50.0f, 1.0f, 0.0f,
			 50.0f, 50.0f,  50.0f, 1.0f, 1.0f,
			-50.0f, 50.0f,  50.0f, 0.0f, 1.0f,

			 //bottom
			-50.0f, -50.0f,  50.0f, 0.0f, 0.0f,
			 50.0f, -50.0f,  50.0f, 1.0f, 0.0f,
			 50.0f, -50.0f, -50.0f, 1.0f, 1.0f,
			-50.0f, -50.0f, -50.0f, 0.0f, 1.0f,
		};

		unsigned int indices[] = {
			0 , 1 , 2 , 2 , 3 , 0 ,
			4 , 5 , 6 , 6 , 7 , 4 ,
			8 , 9 , 10, 10, 11, 8 ,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VertexArray = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, sizeof(float) * 5 * 4 * 6);
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6 * 6);

		m_Shader = std::make_unique<Shader>("res/shaders/Texture.shader");
		m_Shader->Bind();

		m_Texture = std::make_unique<Texture>("res/textures/LEFT.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestCube::~TestCube()
	{
	}

	void TestCube::OnUpdate(float deltaTime)
	{
		
	}

	void TestCube::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		glEnable(GL_CULL_FACE);
		// glCullFace(GL_FRONT);
		glCullFace(GL_BACK);


		m_Texture->Bind();
		m_Shader->Bind();

		const Renderer renderer;
		
		{
			const glm::mat4 m_Proj = glm::ortho(0.0f, (float)960, 0.0f, (float)540, -1000.0f, 1000.0f);
			glm::mat4 m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			model = glm::rotate(model, m_TranslationB.x, glm::vec3(-1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, m_TranslationB.y, glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::rotate(model, m_TranslationB.z, glm::vec3(0.0f, 0.0f, -1.0f));
			const glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
		}

	}

	void TestCube::onImGuiRender()
	{
		ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, -960.0f, 960.0f);
		ImGui::SliderFloat3("TranslationB", &m_TranslationB.x, 0.0f, 6.28f);
		ImGui::Text("Application average %.1f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}

#include "TestBatch.h"

#include "TestCube.h"

#include "Renderer.h"
#include "Texture.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace test
{
	TestBatch::TestBatch() :
		m_TranslationA(200.0f, 200.0f, 0.0f),
		m_TranslationB(300.0f, 200.0f, 0.0f)
	{
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.2f, 0.3f, 0.7f, 1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, -50.0f, 0.0f, 0.2f, 0.3f, 0.7f, 1.0f, 1.0f, 0.0f,0.0f,
			0.0f, 0.0f, 0.0f, 0.2f, 0.3f, 0.7f, 1.0f, 1.0f, 1.0f,   0.0f,
			-50.0f, 0.0f, 0.0f, 0.2f, 0.3f, 0.7f, 1.0f, 0.0f, 1.0f,0.0f,

			20.0f, 20.0f, 0.0f, 0.7f, 0.3f, 0.2f, 1.0f, 0.0f, 0.0f,1.0f, 
			50.0f, 20.0f, 0.0f, 0.7f, 0.3f, 0.2f, 1.0f, 1.0f, 0.0f,1.0f, 
			50.0f, 50.0f, 0.0f, 0.7f, 0.3f, 0.2f, 1.0f, 1.0f, 1.0f, 1.0f,
			20.0f, 50.0f, 0.0f, 0.7f, 0.3f, 0.2f, 1.0f, 0.0f, 1.0f,1.0f, 
		};

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};
		
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		
		m_VertexArray = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, sizeof(float) * 8 * 10);
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(4);
		layout.Push<float>(2);
		layout.Push<float>(1);
		m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
		
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6 * 2);
		
		m_Shader = std::make_unique<Shader>("res/shaders/Texture2.shader");
		m_Shader->Bind();

		m_Texture1 = std::make_unique<Texture>("res/textures/LEFT.png");
		m_Shader->SetUniform1i("u_Texture", 0);
		m_Texture2 = std::make_unique<Texture>("res/textures/RIGHT.png");
		m_Shader->SetUniform1i("u_Texture", 1);
	}

	TestBatch::~TestBatch()
	{
	}

	void TestBatch::OnUpdate(float deltaTime)
	{
		
	}

	void TestBatch::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		const Renderer renderer;
		// const glm::mat4 m_Proj = glm::perspective(glm::radians(90.0f), (float)960/ (float)540, -1.0f, 1.0f);
		const glm::mat4 m_Proj = glm::ortho(0.0f, (float)960, 0.0f, (float)540, -1.0f, 1000.0f);

		const glm::mat4 m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		const glm::mat4 m_Model = glm::translate(glm::mat4(1.0f), m_TranslationA);

		m_Texture1->Bind(0);
		m_Texture2->Bind(1);
		m_Shader->Bind();
		
		{
			m_Shader->SetUniformMat4f("u_Model", m_Model);
			m_Shader->SetUniformMat4f("u_View", m_View);
			m_Shader->SetUniformMat4f("u_Proj", m_Proj);
			renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
		}
		
		// {
		// 	const glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
		// 	const glm::mat4 mvp = m_Proj * m_View * model;
		// 	m_Shader->SetUniformMat4f("u_MVP", mvp);
		// 	renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
		// }
	}

	void TestBatch::onImGuiRender()
	{
		ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("TranslationB", &m_TranslationB.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.1f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}

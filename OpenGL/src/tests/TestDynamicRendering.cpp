#include "TestDynamicRendering.h"

#include <array>

#include "TestCube.h"

#include "Renderer.h"
#include "Texture.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "imgui/imgui.h"

namespace test
{
	struct Vec3
	{
		float x, y, z;
	};

	struct Vec4
	{
		float x, y, z, w;
	};

	struct Vec2
	{
		float x, y;
	};

	struct Vertex
	{
		Vec3 Position;
		Vec4 Color;
		Vec2 TexCoords;
		float TexID;
	};

	static unsigned int* GenerateStandardIndices(const unsigned int count)
	{
		unsigned int* indices = (unsigned int*)_malloca(sizeof(unsigned int) * 6 * count);
		int fourI, sixI;
		for(unsigned int i = 0; i < count; i++)
		{
			sixI = 6 * i;
			fourI = 4 * i;
			indices[0 + sixI] = 0 + fourI;
			indices[1 + sixI] = 1 + fourI;
			indices[2 + sixI] = 2 + fourI;
			indices[3 + sixI] = 2 + fourI;
			indices[4 + sixI] = 3 + fourI;
			indices[5 + sixI] = 0 + fourI;
		}
		return indices;
	}

	static std::array<Vertex, 4> CreateQuad(float x, float y, float TextureID = 0.0f)
	{
		float size = 100.0f;
		Vertex v0;
		v0.Position = { -0.5f * size + x, -0.5f * size + y, 0.0f };
		v0.Color = { 0.2f, 0.3f, 0.7f, 1.0f };
		v0.TexCoords = { 0.0f, 0.0f };
		v0.TexID = TextureID;

		Vertex v1;
		v1.Position = { 0.5f * size + x, -0.5f * size + y, 0.0f };
		v1.Color = { 0.2f, 0.3f, 0.7f, 1.0f };
		v1.TexCoords = { 1.0f, 0.0f };
		v1.TexID = TextureID;

		Vertex v2;
		v2.Position = { 0.5f * size + x, 0.5f * size + y, 0.0f };
		v2.Color = { 0.2f, 0.3f, 0.7f, 1.0f };
		v2.TexCoords = { 1.0f, 1.0f };
		v2.TexID = TextureID;

		Vertex v3;
		v3.Position = { -0.5f * size + x, 0.5f * size + y, 0.0f };
		v3.Color = { 0.2f, 0.3f, 0.7f, 1.0f };
		v3.TexCoords = { 0.0f, 1.0f };
		v3.TexID = TextureID;

		return { v0, v1, v2, v3 };
	}

	TestDynamicRendering::TestDynamicRendering() :
		m_TranslationA(200.0f, 200.0f, 0.0f),
		m_TranslationB(300.0f, 200.0f, 0.0f)
	{
		unsigned int *indices = GenerateStandardIndices(2);
		
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		
		m_VertexArray = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, sizeof(Vertex) * 4 * 2, GL_DYNAMIC_DRAW);
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

	TestDynamicRendering::~TestDynamicRendering()
	{
	}

	void TestDynamicRendering::OnUpdate(float deltaTime)
	{
		
	}

	void TestDynamicRendering::OnRender()
	{
		auto q0 = CreateQuad(glm::sin(m_move/10) * 30, glm::cos(m_move/10) * 10);
		auto q1 = CreateQuad(200.0f, 0.0f, 1.0f);

		Vertex verticies[2 * 4];
		memcpy(verticies, q0.data(), q0.size() * sizeof(Vertex));
		memcpy(verticies + q1.size(), q1.data(), q1.size() * sizeof(Vertex));
		// Set Dynamic Vertex Buffer
		m_VertexBuffer->Bind();
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticies), verticies));

		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		const Renderer renderer;
		// const glm::mat4 m_Proj = glm::perspective(glm::radians(90.0f), (float)960/ (float)540, -1.0f, 1.0f);
		const glm::mat4 m_Proj = glm::ortho(0.0f, (float)960, 0.0f, (float)540, -1000.0f, 1000.0f);
		const glm::mat4 m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		 glm::mat4 m_Model = glm::translate(glm::mat4(1.0f), m_TranslationA);

		m_Model = glm::rotate(m_Model, glm::radians(m_move * 2), glm::vec3(-1.0f, 0.0f, 0.0f));
		m_Model = glm::rotate(m_Model, glm::radians(m_move * 2), glm::vec3(0.0f, -1.0f, 0.0f));
		m_Model = glm::rotate(m_Model, glm::radians(m_move * 2), glm::vec3(0.0f, 0.0f, -1.0f));


		m_Texture1->Bind(0);
		m_Texture2->Bind(1);
		m_Shader->Bind();
		
		{
			m_Shader->SetUniformMat4f("u_Model", m_Model);
			m_Shader->SetUniformMat4f("u_View", m_View);
			m_Shader->SetUniformMat4f("u_Proj", m_Proj);
			renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
		}

		m_move += 0.1f;
	}

	void TestDynamicRendering::onImGuiRender()
	{
		ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("TranslationB", &m_TranslationB.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.1f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("move %.1f", m_move);
	}
}

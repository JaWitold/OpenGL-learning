#pragma once
#include "Test.h"
#include "glm/vec3.hpp"
#include "Renderer.h"
#include "memory"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Texture;

namespace test
{
	class TestBatch : public test::Test{
	public:
		TestBatch();
		~TestBatch();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void onImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture1;
		std::unique_ptr<Texture> m_Texture2;

		glm::vec3 m_TranslationA, m_TranslationB;
	};
}

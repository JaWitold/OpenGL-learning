#pragma once
#include "Test.h"

namespace test
{
	class TestClearColor : public test::Test{
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void onImGuiRender() override;
	private:
		float m_ClearColor[4];
	};
}
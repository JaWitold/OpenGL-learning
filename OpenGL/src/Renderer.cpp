#include "Renderer.h"
#include <iostream>


void GLClearError()
{
	while (glGetError());
}

bool GLLogCall(const char* function, const char* file, const int line)
{
	while (const GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] " << error << " " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Draw(const VertexArray& vertex_array, const IndexBuffer& index_buffer, const Shader& shader) const
{
	shader.Bind();
	vertex_array.Bind();
	index_buffer.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, index_buffer.getCount(), GL_UNSIGNED_INT, nullptr));
}

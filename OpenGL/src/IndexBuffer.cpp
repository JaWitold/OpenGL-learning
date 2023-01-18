#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned count)
	: m_Count(count)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	Bind();
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));

	// Unbind();
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &m_RendererID));
}


void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));

}

void IndexBuffer::Unbind() const 
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
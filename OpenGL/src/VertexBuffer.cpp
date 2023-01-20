#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned size, const GLenum usage)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	Bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, usage));

	// Unbind();
}

VertexBuffer::~VertexBuffer()
{
	// Unbind();
	GLCall(glDeleteBuffers(1, &m_RendererID));
}


void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
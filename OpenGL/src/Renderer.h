#pragma once
#include <GL/glew.h>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, const int line);

class Renderer
{
public:
	inline void Clear() const {GLCall(glClear(GL_COLOR_BUFFER_BIT));};
	void Draw(const VertexArray& vertex_array, const IndexBuffer& index_buffer, const Shader& shader) const;
};

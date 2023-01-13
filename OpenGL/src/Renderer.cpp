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

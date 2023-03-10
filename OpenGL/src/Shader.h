#pragma once
#include <iostream>
#include <unordered_map>
#include <GL/glew.h>

#include "glm/glm.hpp"

struct ShaderProgramSource {
	std::string vertex_source;
	std::string fragment_source;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	//caching for uniform
	mutable std::unordered_map <std::string, GLint> m_UniformLocationCache;
public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//set uniforms
	void SetUniform1i(const std::string& name, int v0);

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1f(const std::string& name, float v0);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	GLint getUniformLocation(const std::string& name) const;
	ShaderProgramSource ParseShader(const std::string& file_path);

	unsigned int CompileShader(unsigned type, const std::string& source);
	unsigned int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);
};
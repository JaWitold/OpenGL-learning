#pragma once
#include <iostream>
#include <unordered_map>

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
	std::unordered_map <std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	int getUniformLocation(const std::string& name);
	ShaderProgramSource ParseShader(const std::string& file_path);

	unsigned int CompileShader(unsigned type, const std::string& source);
	unsigned int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);
};
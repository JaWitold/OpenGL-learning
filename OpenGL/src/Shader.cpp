#include "Shader.h"

#include <string>

#include "Renderer.h"
#include <fstream>
#include "sstream"

Shader::Shader(const std::string& filePath)
	: m_RendererID(0), m_FilePath(filePath)
{
	const ShaderProgramSource source = ParseShader(filePath);
	m_RendererID = CreateShader(source.vertex_source, source.fragment_source);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

int Shader::getUniformLocation(const std::string& name)
{
	if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
	{
		std::cout << "Warning: uniform'" << name << "' does not exists" << std::endl;
	} 
	m_UniformLocationCache[name] = location;
	return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& file_path) {
	std::ifstream stream(file_path, std::ifstream::in);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	ShaderType type = ShaderType::NONE;

	std::string line;
	std::stringstream ss[2];

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			if (type != ShaderType::NONE) {
				ss[(int)type] << line << "\n";
			}
		}

	}

	stream.close();

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(const unsigned int type, const std::string& source) {
	GLCall(const unsigned int id = glCreateShader(type));
	GLCall(const char* src = source.c_str());
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	//Error Handling
	int compilation_status;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &compilation_status));
	if (compilation_status == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		char* message = (char*)_malloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		std::cerr << "FAILED TO COMPILE " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
		std::cerr << message << std::endl;

		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertex_shader, const std::string& fragment_shader) {
	GLCall(const unsigned int program = glCreateProgram());
	const unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
	const unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));

	GLCall(glLinkProgram(program));

	//Error Handling
	int linking_status;
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &linking_status));
	if (linking_status == GL_FALSE) {
		int length;
		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));

		char* message = (char*)_malloca(length * sizeof(char));
		GLCall(glGetProgramInfoLog(program, length, &length, message));

		std::cerr << "FAILED TO LINK PROGRAM" << std::endl;
		std::cerr << message << std::endl;

		GLCall(glDeleteProgram(program));
		return 0;
	}

	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	//Error Handling
	int validation_status;
	GLCall(glGetProgramiv(program, GL_VALIDATE_STATUS, &validation_status));
	if (validation_status == GL_FALSE) {
		int length;
		GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));

		char* message = (char*)_malloca(length * sizeof(char));
		GLCall(glGetProgramInfoLog(program, length, &length, message));

		std::cerr << "FAILED TO LINK PROGRAM" << std::endl;
		std::cerr << message << std::endl;

		GLCall(glDeleteProgram(program));
		return 0;
	}
	return program;
}


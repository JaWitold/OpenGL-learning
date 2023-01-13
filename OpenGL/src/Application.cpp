

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	while (glGetError());
}

static bool GLLogCall(const char* function, const char* file, const int line)
{
	while(const GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] " << error << " " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

struct shader_program_source {
	std::string vertex_source;
	std::string fragment_source;
};

static shader_program_source parse_shader(const std::string& file_path) {
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
		} else {
			if (type != ShaderType::NONE) {
				ss[(int)type] << line  << "\n";
			}
		}

	}

	stream.close();

	return { ss[0].str(), ss[1].str() };
}

static unsigned int compile_shader(const unsigned int type, const std::string& source) {
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
		
		char* message = (char*) _malloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		std::cerr << "FAILED TO COMPILE " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
		std::cerr << message << std::endl;

		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

static int create_shader(const std::string& vertex_shader, const std::string& fragment_shader) {
	GLCall(const unsigned int program = glCreateProgram());
	GLCall(const unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader));
	GLCall(unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader));

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
		return -1;
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
		return -1;
	}
	return (int)program;
}

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK) {
		return -2;
	}

	constexpr float positions[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f,
	};

	const unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	//Generate vertex array object cause it is mandatory to be initialized in CORE profile
	unsigned int vertex_array;
	GLCall(glGenVertexArrays(1, &vertex_array));
	GLCall(glBindVertexArray(vertex_array));

	unsigned int vertex_buffer;
	GLCall(glGenBuffers(1, &vertex_buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2, positions, GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

	unsigned int index_buffer;
	GLCall(glGenBuffers(1, &index_buffer));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW));

	const shader_program_source source = parse_shader("res/shaders/Basic.shader");
	const unsigned int shader = create_shader(source.vertex_source, source.fragment_source);

	GLCall(const int location = glGetUniformLocation(shader, "u_Color"));

	GLCall(glUseProgram(shader));

	GLCall(glBindVertexArray(0));
	GLCall(glUseProgram(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	float r = 0.0f;
	float increment = 0.01f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

		GLCall(glBindVertexArray(vertex_array));
		
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		if (r > 1.0f || r < 0.0f)
			increment = -increment;
		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	GLCall(glDeleteProgram(shader));
	glfwTerminate();
	return 0;
}
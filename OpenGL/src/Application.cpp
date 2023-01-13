

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(std::string filePath) {
    std::ifstream stream(filePath, std::ifstream::in);

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

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //Error Handling
    int compilation_status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compilation_status);
    if (compilation_status == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        
        char* message = (char*) _malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cerr << "FAILED TO COMPILE " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cerr << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static int CreateShader(const std::string& vertesShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertesShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);

    //Error Handling
    int linking_status;
    glGetProgramiv(program, GL_LINK_STATUS, &linking_status);
    if (linking_status == GL_FALSE) {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*)_malloca(length * sizeof(char));
        glGetProgramInfoLog(program, length, &length, message);

        std::cerr << "FAILED TO LINK PROGRAM" << std::endl;
        std::cerr << message << std::endl;

        glDeleteProgram(program);
        return -1;
    }

    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    //Error Handling
    int validation_status;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &validation_status);
    if (validation_status == GL_FALSE) {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*)_malloca(length * sizeof(char));
        glGetProgramInfoLog(program, length, &length, message);

        std::cerr << "FAILED TO LINK PROGRAM" << std::endl;
        std::cerr << message << std::endl;

        glDeleteProgram(program);
        return -1;
    }
    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        return -2;
    }
    
    float positions[6] = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f};

    //Create buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer);

    //Select buffer and set it type - Array buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    //Put data into buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    std::string vertexShader, fragmentShader;
    std::string path = "res/shaders/Basic.shader";
    ShaderProgramSource source = ParseShader(path);

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
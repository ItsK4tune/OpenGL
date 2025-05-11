#include "util/construct.h"

std::string loadShaderSource(const char *filepath)
{
    std::string fullPath = std::string("../") + filepath;

    std::ifstream file(fullPath);
    if (!file.is_open())
    {
        std::cerr << "Cannot read shader file: " << fullPath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void createVAO(GLuint &vao, GLuint &vbo, GLuint &ebo, float *vertices, unsigned int *indices)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

GLuint compileShader(GLenum type, const std::string &src)
{
    GLuint shader = glCreateShader(type);
    const char *csrc = src.c_str();
    glShaderSource(shader, 1, &csrc, nullptr);
    glCompileShader(shader);
    return shader;
}

GLuint createProgram(const char *vsPath, const char *fsPath)
{
    std::string vsCode = loadShaderSource(vsPath);
    std::string fsCode = loadShaderSource(fsPath);
    GLuint vs = compileShader(GL_VERTEX_SHADER, vsCode);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsCode);
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
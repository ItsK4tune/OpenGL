#include "util/util.h"

std::string vertSrc = loadShaderSource("shader/basic.vert");
std::string fragSrc = loadShaderSource("shader/basic.frag");

const char *vSrc = vertSrc.c_str();
const char *fSrc = fragSrc.c_str();

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL renderWindow", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();

    // VAO tối thiểu để OpenGL core không bị crash
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Compile shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vSrc, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fSrc, NULL);
    glCompileShader(fs);

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    auto start = std::chrono::steady_clock::now();

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        auto now = std::chrono::steady_clock::now();
        float time = std::chrono::duration<float>(now - start).count();

        glUseProgram(program);
        glUniform2f(glGetUniformLocation(program, "iResolution"), width, height);
        glUniform1f(glGetUniformLocation(program, "iTime"), time);

        glDrawArrays(GL_TRIANGLES, 0, 3); // Fullscreen triangle

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

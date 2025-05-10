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

    GLuint vao, vbo;
    float screenW = 800.0f, screenH = 600.0f;
    float quadW = 400.0f, quadH = 400.0f;

    // Tính tọa độ normalized device coordinates (NDC)
    float left = (screenW / 2.0f - quadW / 2.0f) / (screenW / 2.0f) - 1.0f;
    float right = (screenW / 2.0f + quadW / 2.0f) / (screenW / 2.0f) - 1.0f;
    float top = 1.0f - (screenH / 2.0f - quadH / 2.0f) / (screenH / 2.0f);
    float bottom = 1.0f - (screenH / 2.0f + quadH / 2.0f) / (screenH / 2.0f);

    // Vertex positions and UVs
    float vertices[] = {
        // pos (x, y)      // uv
        left, bottom, 0.0f, 0.0f,
        right, bottom, 1.0f, 0.0f,
        right, top, 1.0f, 1.0f,
        left, top, 0.0f, 1.0f};

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0};

    GLuint ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint program = glCreateProgram();
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vSrc, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fSrc, NULL);
    glCompileShader(fs);
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

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

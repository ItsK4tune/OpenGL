#include "util/construct.hpp"
#include "util/camera.hpp"
#include "util/input.hpp"

int initWidth = 0;
int initHeight = 0;

GLFWwindow *createWindow(int width, int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(width, height, "OpenGL renderWindow", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    gladLoadGL();

    initWidth = width;
    initHeight = height;

    return window;
}

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

const float TARGET_ASPECT = 1.0f / 1.0f;
bool resizing = false;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    if (resizing)
        return;

    resizing = true;

    if (width != initWidth)
    {
        int newHeight = static_cast<int>(width / TARGET_ASPECT);
        glfwSetWindowSize(window, width, newHeight);
        initWidth = width;
    }
    if (height != initHeight)
    {
        int newWidth = static_cast<int>(height * TARGET_ASPECT);
        glfwSetWindowSize(window, newWidth, height);
        initHeight = height;
    }

    int finalWidth, finalHeight;
    glfwGetFramebufferSize(window, &finalWidth, &finalHeight);
    glViewport(0, 0, finalWidth, finalHeight);
    // camera.updateFromWindowSize(finalWidth, finalHeight);

    resizing = false;
}

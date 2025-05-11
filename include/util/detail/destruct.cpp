#include "util/destruct.h"

void endRender(GLuint &program, GLuint &vao, GLFWwindow *window)
{
    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glfwDestroyWindow(window);
    glfwTerminate();
}
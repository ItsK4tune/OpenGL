#include "util/input.hpp"

float rotX = 0.0f;
float rotY = 0.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    const float rotSpeed = 5.0f;
    const float moveSpeed = 0.1f;

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (key == GLFW_KEY_UP)
            rotX -= rotSpeed;
        else if (key == GLFW_KEY_DOWN)
            rotX += rotSpeed;
        else if (key == GLFW_KEY_LEFT)
            rotY -= rotSpeed;
        else if (key == GLFW_KEY_RIGHT)
            rotY += rotSpeed;

        else if (key == GLFW_KEY_W)
            cameraPos += moveSpeed * cameraFront;
        else if (key == GLFW_KEY_S)
            cameraPos -= moveSpeed * cameraFront;
        else if (key == GLFW_KEY_A)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * moveSpeed;
        else if (key == GLFW_KEY_D)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * moveSpeed;
        else if (key == GLFW_KEY_SPACE)
            cameraPos += moveSpeed * cameraUp;
        else if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)
            cameraPos -= moveSpeed * cameraUp;

        else if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
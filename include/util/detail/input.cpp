#include "util/input.hpp"
#include "util/camera.hpp"

float rotX = 0.0f;
float rotY = 0.0f;
float scaleFactor = 1.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse = true;

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

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
    {
        firstMouse = true;
        return;
    }

    const float sensitivity = 0.1f;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    rotY += xoffset * sensitivity;
    rotX += yoffset * sensitivity;

    if (rotX > 89.0f)
        rotX = 89.0f;
    if (rotX < -89.0f)
        rotX = -89.0f;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    const float scaleSpeed = 0.1f;
    scaleFactor += yoffset * scaleSpeed;

    if (scaleFactor < 0.1f)
        scaleFactor = 0.1f;
    if (scaleFactor > 10.0f)
        scaleFactor = 10.0f;
}
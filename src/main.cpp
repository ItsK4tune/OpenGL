#include "util/construct.hpp"
#include "util/destruct.hpp"
#include "util/affine.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    gladLoadGL();

    float vertices[] = {
        // Positions            // Colors (x, y, z, w)
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 0
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // 1
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,   // 2
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  // 3
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,  // 4
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // 5
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,    // 6
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f    // 7
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0, // mặt dưới
        4, 5, 6, 6, 7, 4, // mặt trên
        0, 1, 5, 5, 4, 0, // mặt phía trước
        2, 3, 7, 7, 6, 2, // mặt phía sau
        1, 2, 6, 6, 5, 1, // mặt bên phải
        4, 7, 3, 3, 0, 4  // mặt bên trái
    };

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint program = createProgram("shader/cube/cube.vert", "shader/cube/cube.frag");

    // GLuint texture;
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_2D, texture);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // int w, h, channels;
    // stbi_set_flip_vertically_on_load(true);
    // unsigned char *img = stbi_load("../shader/texture/bookshelf.jpg", &w, &h, &channels, 0);

    // if (img)
    // {
    //     GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
    //     glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, img);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    //     stbi_image_free(img);
    // }
    // else
    // {
    //     std::cerr << "Failed to load texture\n";
    //     return -1;
    // }

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program);

        float time = glfwGetTime();
        glm::mat4 model = createRotationMatrix(glm::vec3(time, time, time));

        GLuint modelLoc = glGetUniformLocation(program, "uModel");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    endRender(program, vao, window);
    return 0;
}

#include "util/construct.hpp"
#include "util/destruct.hpp"
#include "util/affine.hpp"
#include "util/input.hpp"
#include "util/camera.hpp"
#include "util/objLoader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

int main()
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Camera camera(glm::vec3(3.0f, 3.0f, 3.0f), // Vị trí camera
                  glm::vec3(0.0f, 0.0f, 0.0f), // Mục tiêu của camera (góc nhìn)
                  glm::vec3(0.0f, 1.0f, 0.0f), // Hướng "up" của camera
                  45.0f,                       // Góc nhìn (FOV) của camera
                  800.0f / 600.0f,             // Tỷ lệ khung hình (aspect ratio)
                  0.1f,                        // Mặt gần (near plane)
                  100.0f);                     // Mặt xa (far plane)

    std::string filepath = "../shader/cat/Cat_LP3.obj";
    if (loadObj(filepath, vertices, indices))
    {
        std::cout << "OBJ loaded successfully.\n";
    }
    else
    {
        std::cerr << "Failed to load OBJ.\n";
    }

    GLFWwindow *window = createWindow(400, 400);

    GLuint vao, vaoEdge, vbo, ebo, eboEdge;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    GLuint program = createProgram("shader/cat/cat.vert", "shader/cat/cat.frag");

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    GLuint posAttrib = glGetAttribLocation(program, "aPosition");
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Position));
    glEnableVertexAttribArray(posAttrib);

    GLuint texAttrib = glGetAttribLocation(program, "aTexCoords");
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(texAttrib);

    GLuint normAttrib = glGetAttribLocation(program, "aNormal");
    glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(normAttrib);

    int width, height, nrChannels;
    // stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("../shader/cat/Cat_Basecolor.png", &width, &height, &nrChannels, 0);
    GLuint texture;
    if (data)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else
    {
        std::cerr << "Failed to load texture\n";
    }

    glUseProgram(program);
    GLuint textureLocation = glGetUniformLocation(program, "texture1");
    glUniform1i(textureLocation, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        camera.setPosition(cameraPos);
        camera.setTarget(cameraPos + cameraFront);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 affine = createAffineTransformMatrix(
            glm::vec3(scaleFactor),                                  // scale đều các chiều
            glm::vec3(glm::radians(rotX), glm::radians(rotY), 0.0f), // rotation XYZ
            glm::vec3(0)                                             // tịnh tiến
        );
        model = affine * model;

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 mvp = projection * view * model;

        GLuint mvpLocation = glGetUniformLocation(program, "uMVP");
        glUseProgram(program);
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    endRender(program, vao, window);
    return 0;
}

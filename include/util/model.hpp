#ifndef MODEL_HPP
#define MODEL_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstddef>

struct Vertex
{
    float Position[3];
    float TexCoords[2];
    float Normal[3];
};

class Model
{
public:
    Model(const std::string &path);
    ~Model();
    void Draw(GLuint shaderProgram);

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    GLuint VAO, VBO, EBO;

    bool loadObj(const std::string &path);
    void setupMesh();
};

#endif
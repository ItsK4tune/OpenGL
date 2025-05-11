#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::string loadShaderSource(const char *filepath);
void createVAO(GLuint &vao, GLuint &vbo, GLuint &ebo, float *vertices, unsigned int *indices);
GLuint compileShader(GLenum type, const std::string &src);
GLuint createProgram(const char *vsPath, const char *fsPath);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#endif
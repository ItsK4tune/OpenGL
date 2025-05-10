#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::string loadShaderSource(const char *filepath);

void createVAO(GLuint &vao, GLuint &vbo, GLuint &ebo, float *vertices, unsigned int *indices);

#endif
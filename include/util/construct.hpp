#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

extern int initWidth;
extern int initHeight;

GLFWwindow *createWindow(int width, int height);
std::string loadShaderSource(const char *filepath);
GLuint compileShader(GLenum type, const std::string &src);
GLuint createProgram(const char *vsPath, const char *fsPath);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#endif
#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "shaderprogram.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, ShaderProgram &shaderProgram);

#endif // MAIN_H

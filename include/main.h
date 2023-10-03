#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"
#include "stb_image.h"

void initGlfw(int gl_major, int gl_minor, bool core_profile);
int createWindow(GLFWwindow ** window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, ShaderProgram &shaderProgram);

#endif // MAIN_H

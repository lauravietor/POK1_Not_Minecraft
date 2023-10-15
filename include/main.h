#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <random>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"
#include "stb_image.h"
#include "world.h"

void initGlfw(int gl_major, int gl_minor, bool core_profile);
int createWindow(GLFWwindow ** window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

void add_top_face(int x, int y, int h, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
void add_west_face(int x, int y, int h, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
void add_east_face(int x, int y, int h, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
void add_south_face(int x, int y, int h, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
void add_north_face(int x, int y, int h, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
void add_vertex(std::vector<GLfloat> &vertices, GLfloat x, GLfloat y, GLfloat z, GLfloat n_x, GLfloat n_y, GLfloat n_z, GLfloat t_x, GLfloat t_y, GLfloat t_z);

#endif // MAIN_H

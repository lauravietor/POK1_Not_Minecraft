#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <glad/glad.h>

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

class ShaderProgram
{
public:
	GLuint ID; // OpenGL ID of this shader program
	ShaderProgram(const char* vertexPath, const char* fragmentPath);

	// Select this shader program for use
	void use();

	void setUniform4f(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void setUniform3f(const std::string &name, GLfloat x, GLfloat y, GLfloat z);
};

#endif // SHADERPROGRAM_H

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	void setUniform2f(const std::string &name, GLfloat x, GLfloat y);
	void setUniform1f(const std::string &name, GLfloat x);
	void setUniform1i(const std::string &name, GLint n);
	void setUniformMat4x4(const std::string &name, glm::mat4x4 &matrix);
};

#endif // SHADERPROGRAM_H

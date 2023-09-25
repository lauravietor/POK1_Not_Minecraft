#include "shaderprogram.h"

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath)
{

	// Retrieve the source for both shaders

	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::stringstream vShaderStream, fShaderStream;
	vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Read vertex shader
		vShaderFile.open(vertexPath);
		vShaderStream << vShaderFile.rdbuf();
		vShaderFile.close();
		vertexCode   = vShaderStream.str();

		// Read fragment shader
		fShaderFile.open(fragmentPath);
		fShaderStream << fShaderFile.rdbuf();
		fShaderFile.close();
		fragmentCode = fShaderStream.str();
	}
	catch (...)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	// Compile both shaders

	GLuint vertex, fragment;
	int success;
	char infoLog[512];

	// vertex Shader
	const char * vShaderCode = vertexCode.c_str();
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	// Check for errors
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// fragment Shader
	const char * fShaderCode = fragmentCode.c_str();
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	// Check for errors
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// Link shaders

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	// Check for errors
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Shaders are not needed anymore once they're linked
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void ShaderProgram::use()
{
	glUseProgram(ID);
}

void ShaderProgram::setUniform4f(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void ShaderProgram::setUniform3f(const std::string &name, GLfloat x, GLfloat y, GLfloat z)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void ShaderProgram::setUniform2f(const std::string &name, GLfloat x, GLfloat y)
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void ShaderProgram::setUniform1f(const std::string &name, GLfloat x)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), x);
}

void ShaderProgram::setUniform1i(const std::string &name, GLint n)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), n);
}

void ShaderProgram::setUniformMat4x4(const std::string &name, glm::mat4x4 &matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

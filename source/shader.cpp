#include "shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

Xplor::Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string vertexCode, fragmentCode;
	std::ifstream vertexShaderFile, fragmentShaderFile;

	// enable exceptions on files streams
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// Read Files
	try
	{
		vertexShaderFile.open(vertexShaderPath);
		fragmentShaderFile.open(fragmentShaderPath);
		std::stringstream vertexShaderStream, fragmentShaderStream;
		// read file contents into streams
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// convert the stream into a string
		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure exception)
	{
		std::cout << "ERROR: Shader file could not be read" << std::endl;
	}

	//-- Compile Input Shaders
	// Vertex Shader
	auto vertex = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
	// Fragment Shader
	auto fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

	//-- Link the Shader Program
	shaderID = glCreateProgram();
	glAttachShader(shaderID, vertex);
	glAttachShader(shaderID, fragment);
	glLinkProgram(shaderID);
	
	// Check for linking issues
	int linkSuccess;
	char infoLog[512];
	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &linkSuccess);
	if (!linkSuccess)
	{
		glGetProgramInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << "ERROR: SHADER_PROGRAM Link FAILED\n" << infoLog << std::endl;
	}


	glDeleteShader(vertex);
	glDeleteProgram(fragment);
}

Xplor::Shader::~Shader()
{
	glDeleteProgram(shaderID);
}

void Xplor::Shader::useProgram()
{
	glUseProgram(shaderID);
}

void Xplor::Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), static_cast<int>(value));
}

void Xplor::Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}

void Xplor::Shader::setFloat(const std::string& name, float value) const
{
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}

uint16_t Xplor::Shader::compileShader(int shaderType, const char *shaderSource) const
{
	unsigned int id;
	int success;
	char infoLog[512];

	switch (shaderType)
	{
	case GL_VERTEX_SHADER:
		id = glCreateShader(GL_VERTEX_SHADER);
		break;
	case GL_FRAGMENT_SHADER:
		id = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	default:
		break;
	}

	glShaderSource(id, 1, &shaderSource, NULL);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR: SHADER_FRAGMENT Compilation FAILED\n" << infoLog << std::endl;
	}

	return id;
}
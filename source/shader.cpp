#include "shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

Xplor::Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	m_vertexPath = vertexShaderPath;
	m_fragmentPath = fragmentShaderPath;
}

uint32_t Xplor::Shader::getID()
{
	return m_shaderID;
}

void Xplor::Shader::useProgram()
{
	glUseProgram(m_shaderID);
}

void Xplor::Shader::endProgram()
{
	glUseProgram(0);
}


void Xplor::Shader::setUniform(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), value);
	m_uniformInts.push_back(std::make_tuple(name, value));
}

void Xplor::Shader::setUniform(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Xplor::Shader::setUniform(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), static_cast<int>(value));
}

void Xplor::Shader::setUniform(const std::string& name, float value) const
{
	glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), static_cast<GLint>(value));
}

void Xplor::Shader::Delete()
{
	std::cout << "Shader Program Destroyed" << std::endl;
	glDeleteProgram(m_shaderID);
}

GLuint Xplor::Shader::compileShader(int shaderType, const char *shaderSource) const
{
	GLuint id;
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
		assert(false && "Error unexpected shader type");
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

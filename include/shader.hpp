#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "xplor_types.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>


namespace Xplor
{
	class Shader
	{
	public:
		Shader()
		{

		}

		/// <summary>
		/// Shader constructor
		/// </summary>
		/// <param name="vertexShaderPath">Absolute path to vertex shader file.</param>
		/// <param name="fragmentShaderPath">Absolute path to fragment shader file.</param>
		Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

		void init()
		{
			if (m_fragmentPath.empty() || m_vertexPath.empty())
			{
				assert(false && "Shader paths are not defined");
				return;
			}
				

			std::string vertexCode, fragmentCode;
			std::ifstream vertexShaderFile, fragmentShaderFile;

			// enable exceptions on files streams
			vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			// Read Files
			try
			{
				vertexShaderFile.open(m_vertexPath);
				fragmentShaderFile.open(m_fragmentPath);
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
				__debugbreak(); // Windows only
				std::cout << "ERROR: Shader file could not be read" << std::endl;
			}

			//-- Compile Input Shaders
			// Vertex Shader
			auto vertex = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
			// Fragment Shader
			auto fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

			//-- Link the Shader Program
			m_shaderID = glCreateProgram();
			glAttachShader(m_shaderID, vertex);
			glAttachShader(m_shaderID, fragment);
			glLinkProgram(m_shaderID);

			// Check for linking issues
			int linkSuccess;
			char infoLog[512];
			glLinkProgram(m_shaderID);
			glGetProgramiv(m_shaderID, GL_LINK_STATUS, &linkSuccess);
			if (!linkSuccess)
			{
				glGetProgramInfoLog(m_shaderID, 512, NULL, infoLog);
				std::cout << "ERROR: SHADER_PROGRAM Link FAILED\n" << infoLog << std::endl;
			}


			glDeleteShader(vertex);
			glDeleteProgram(fragment);
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		uint32_t getID();

		/// <summary>
		/// Use the current shader program
		/// </summary>
		void useProgram();

		/// <summary>
		/// Call glUseProgram(0) to stop usage of current shader
		/// </summary>
		void endProgram();

		/// <summary>
		/// Defines a integer value for the shader
		/// </summary>
		/// <param name="name"></param>
		/// <param name="value"></param>
		void setUniform(const std::string& name, int value);

		/// <summary>
		/// Defines a 4x4 glm matrix for the shader
		/// </summary>
		/// <param name="name"></param>
		/// <param name="value"></param>
		void setUniform(const std::string& name, glm::mat4 value) const;

		
		void PassUniformInts()
		{
			//useProgram();
			for (auto pair : m_uniformInts)
			{
				glUniform1i(glGetUniformLocation(m_shaderID, std::get<0>(pair).c_str()), std::get<1>(pair));

			}
			//endProgram();
		}

		/// <summary>
		/// Defines a boolean uniform for the shader
		/// </summary>
		/// <param name="name"></param>
		/// <param name="value"></param>
		void setUniform(const std::string& name, bool value) const;


		/// <summary>
		/// Defines a float uniform for the shader
		/// </summary>
		/// <param name="name"></param>
		/// <param name="value"></param>
		void setUniform(const std::string& name, float value) const;

		void Delete();

		json Serialize() const
		{
			return {
				{"vertexPath", m_vertexPath},
				{"fragmentPath", m_fragmentPath},
				{"uniform ints", m_uniformInts}
			};
		}

		void Deserialize(const json& j)
		{
			m_vertexPath = j.at("vertexPath").get<std::string>();
			m_fragmentPath = j.at("fragmentPath").get<std::string>();
			m_uniformInts = j.at("uniform ints");
			PassUniformInts(); // push all the ints to the shader
		}


	protected:
		// Program ID
		uint32_t m_shaderID{};

		GLuint compileShader(int shaderType, const char * shaderSource) const;

	private:
		std::string m_vertexPath{};
		std::string m_fragmentPath{};

		std::vector<std::tuple<std::string, int>> m_uniformInts{};



	};// End Shade Class
}; // End namespace

#endif // !SHADER_H

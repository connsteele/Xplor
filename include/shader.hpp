#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>


namespace Xplor
{
	class Shader
	{
	public:
		/// <summary>
		/// Shader constructor
		/// </summary>
		/// <param name="vertexShaderPath">Absolute path to vertex shader file.</param>
		/// <param name="fragmentShaderPath">Absolute path to fragment shader file.</param>
		Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

		~Shader();

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
		void setUniform(const std::string& name, int value) const;

		/// <summary>
		/// Defines a 4x4 glm matrix for the shader
		/// </summary>
		/// <param name="name"></param>
		/// <param name="value"></param>
		void setUniform(const std::string& name, glm::mat4 value) const;

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


	protected:
		// Program ID
		uint32_t shaderID;

		uint16_t compileShader(int shaderType, const char * shaderSource) const;


	};// End Shade Class
}; // End namespace

#endif // !SHADER_H

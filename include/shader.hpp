#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

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
		/// Use the current shader program
		/// </summary>
		void useProgram();

		//--- Perhaps refactor these to use generics
		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		/// <param name="value"></param>
		void setBool(const std::string &name, bool value) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		/// <param name="value"></param>
		void setInt(const std::string &name, int value) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		/// <param name="value"></param>
		void setFloat(const std::string &name, float value) const;
		// --- End refactor zone

	protected:
		// Program ID
		uint32_t shaderID;

		uint16_t compileShader(int shaderType, const char * shaderSource) const;


	};// End Shade Class
}; // End namespace

#endif // !SHADER_H

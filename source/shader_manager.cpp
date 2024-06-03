#include "shader_manager.hpp"

namespace Xplor
{
	ShaderManager::ShaderManager()
	{
	}

	std::shared_ptr<Shader> ShaderManager::createShader(const std::string& name, const std::string& vertex_path, const std::string& fragment_path)
	{
		// Check if a shader with the name already exists
		auto iterator = m_name_to_shader.find(name);
		if (iterator != m_name_to_shader.end())
		{
			throw std::runtime_error("Shader with name '" + name + "' already exists.");
		}

		// Create the shader
		auto shader = std::make_shared<Shader>(vertex_path.c_str(), fragment_path.c_str());
		shader->init();
		m_name_to_shader[name] = shader;

		return shader;
	}

	std::shared_ptr<Shader> ShaderManager::createShader(const ShaderBasics& info)
	{
		// Check if a shader with the name already exists
		auto iterator = m_name_to_shader.find(info.name);
		if (iterator != m_name_to_shader.end())
		{
			throw std::runtime_error("Shader with name '" + info.name + "' already exists.");
		}

		// Create the shader
		auto shader = std::make_shared<Shader>(info.vertex_path.c_str(), info.fragment_path.c_str());
		shader->init();
		m_name_to_shader[info.name] = shader;

		return shader;
	}


	bool ShaderManager::findShader(const std::string& name, std::shared_ptr<Shader>& out_shader) const
	{
		auto iterator = m_name_to_shader.find(name);
		if (iterator != m_name_to_shader.end())
		{
			out_shader = iterator->second;
			return true;
		}
		
		std::cout << "No shader with name: '" << name << "' found." << std::endl;
		return false;
	}
	
}


#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "manager.hpp"
#include "shader.hpp"

namespace Xplor
{
	struct ShaderInfo
	{
		std::string name;
		std::string vertex_path;
		std::string fragment_path;
	};

	class ShaderManager : public Manager<ShaderManager>
	{
		// Manager used shaders allowing them to be constructed once and re-used when neeeded

	public:
		ShaderManager();
		//static std::shared_ptr<ShaderManager> getInstance();

		std::shared_ptr<Shader> createShader(const std::string& name, const std::string& vertex_path, const std::string& fragment_path);
		std::shared_ptr<Shader> createShader(const ShaderInfo& info);
		bool findShader(const std::string& name, std::shared_ptr<Shader>& out_shader) const;

	protected:

	private:
		//static std::shared_ptr<ShaderManager> m_instance;
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_name_to_shader{};
		std::unordered_map<int, std::string> m_id_to_name{};

		

	};	
}
#pragma once

#include <bitset>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Xplor {

	enum class ImageFormat
	{
		jpg = 0,
		png = 1
	};


	struct CameraVectors {
		glm::vec3 cameraPosition{};
		glm::vec3 cameraFront{};
		glm::vec3 cameraUp{};
	};

	enum class GameObjectType
	{
		GameObject = 0,
		PropObject = 1
	};

	static float* DeserializeFloatArray(const nlohmann::json& j, size_t& out_size) {
		const auto& vec = j.get<std::vector<float>>();
		out_size = vec.size();
		float* arr = new float[out_size];
		std::copy(vec.begin(), vec.end(), arr);
		return arr;
	}

	static unsigned int* DeserializeUIntArray(const nlohmann::json& j, size_t& out_size) {
		const auto& vec = j.get<std::vector<float>>();
		out_size = vec.size();
		unsigned int* arr = new unsigned int[out_size];
		std::copy(vec.begin(), vec.end(), arr);
		return arr;
	}

}; // end namespace
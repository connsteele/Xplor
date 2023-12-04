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

}; // end namespace
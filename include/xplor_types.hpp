#pragma once

#include <bitset>
#include <glm/glm.hpp>

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

}; // end namespace
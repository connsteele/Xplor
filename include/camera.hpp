#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "xplor_types.hpp"
#include "window_manager.hpp"


namespace Xplor {
	class Camera
	{
	public:
		CameraVectors m_vectors;
		glm::vec3 m_target;
		// camera target can be computed from camera front and camera position
		float m_speed{};
		glm::mat4 m_view_matrix{};
		glm::mat4 m_projection_matrix{};

		Camera(CameraVectors cameraVectors, float speed, float fov);
		void Update(const float delta_time);

	private:
		

	}; // end class
}; // end namespace
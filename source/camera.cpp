#include "camera.hpp"

#include "GLFW/glfw3.h"

Xplor::Camera::Camera(Xplor::CameraVectors cameraVecs, float speed, float fov)
{
	m_vectors = cameraVecs;
	m_target = cameraVecs.camera_position + cameraVecs.camera_front;
	m_speed = speed;

	// Get the current aspect ratio
	auto windowManager = WindowManager::getInstance();
	auto window = windowManager->getWindow();
	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

	// Compute matrices
	m_view_matrix = glm::lookAt(m_vectors.camera_position, m_target, m_vectors.camera_up);

	float nearPlane = 0.1f;
	float farPlane = 100.f;
	m_projection_matrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}

void Xplor::Camera::Update(const float delta_time)
{
    auto windowManager = WindowManager::getInstance();

    //---- Translation
    float camera_final_speed = m_speed * delta_time;
    windowManager->ProcessInputs(m_vectors.camera_position, m_vectors.camera_front, m_vectors.camera_up, camera_final_speed);

    //---- Rotation
    static float pitch = 0.0f;
    static float yaw = -90.0f;
    float offsetX, offsetY;
    windowManager->GetMouseOffsets(offsetX, offsetY);
    yaw += offsetX;
    pitch += offsetY;
    // Contrain the pitch to stop a lookAt flip
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    m_vectors.camera_front = glm::normalize(direction);

    m_target = m_vectors.camera_position + m_vectors.camera_front; // Needs to be recomputed after inputs are updated
    
    //---- Compute Matrices
    m_view_matrix = glm::lookAt(m_vectors.camera_position, m_target, m_vectors.camera_up);

    float windowFOV;
    windowManager->GetFOV(windowFOV);
    m_projection_matrix = glm::perspective(glm::radians(windowFOV), 1280.f / 720.f, 0.1f, 100.f);
}

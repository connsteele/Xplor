#include "camera.hpp"

#include "GLFW/glfw3.h"

Xplor::Camera::Camera(Xplor::CameraVectors cameraVecs, float speed, float fov)
{
	m_vectors = cameraVecs;
	m_target = cameraVecs.cameraPosition + cameraVecs.cameraFront;
	m_speed = speed;

	// Get the current aspect ratio
	auto windowManager = WindowManager::GetInstance();
	auto window = windowManager->GetWindow();
	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

	// Compute matrices
	m_viewMatrix = glm::lookAt(m_vectors.cameraPosition, m_target, m_vectors.cameraUp);

	float nearPlane = 0.1f;
	float farPlane = 100.f;
	m_projectionMatrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}

void Xplor::Camera::Update(float deltaTime)
{
    auto windowManager = WindowManager::GetInstance();

    //---- Translation
    float cameraFinalSpeed = m_speed * deltaTime;
    windowManager->ProcessInputs(m_vectors.cameraPosition, m_vectors.cameraFront, m_vectors.cameraUp, cameraFinalSpeed);

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
    m_vectors.cameraFront = glm::normalize(direction);

    m_target = m_vectors.cameraPosition + m_vectors.cameraFront; // Needs to be recomputed after inputs are updated
    
    //---- Compute Matrices
    m_viewMatrix = glm::lookAt(m_vectors.cameraPosition, m_target, m_vectors.cameraUp);

    float windowFOV;
    windowManager->GetFOV(windowFOV);
    m_projectionMatrix = glm::perspective(glm::radians(windowFOV), 1280.f / 720.f, 0.1f, 100.f);
}

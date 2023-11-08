#pragma once

#include <glad/glad.h>
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "string"

enum class VsyncInterval {
	Off = 0,
	On = 1,
	DoubleBuffered = 2,
	TripleBuffered = 3,
};

class WindowManager
{

public:

	void Init();

	void Update();

	/// <summary>
	/// Need to move the camera data out of here. This is a temporary measure
	/// </summary>
	/// <param name="camerPos"></param>
	/// <param name="cameraFront"></param>
	/// <param name="cameraSpee"></param>
	void ProcessInputs(glm::vec3& camerPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float cameraSpee);

	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);

	void UpdateMousePosition(double xpos, double ypos);

	void CaptureCursor();

	void PollEvents();

	void Shutdown();

	void SetVsync(VsyncInterval interval);

	void PrintHardwareInfo();

	void GetMouseOffsets(float& offsetX, float& offsetY);

	// Not the safest behavior here
	GLFWwindow* GetWindow()
	{
		return m_window;
	}

	// Should make this private
	

private:
	const std::string resources = "..//resources";
	GLFWwindow* m_window{};
	float m_cursorOffsetX{}, m_cursorOffsetY{};
	

}; // end class


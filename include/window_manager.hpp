#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "string"
#include <memory>

enum class Vsync {
	Off = 0,
	On = 1,
	DoubleBuffered = 2,
	TripleBuffered = 3,
};

class WindowManager
{

public:

	void Init(int windowWidth, int windowHeight, bool fullscreen);

	void Update();

	/// <summary>
	/// Need to move the camera data out of here. This is a temporary measure
	/// </summary>
	/// <param name="camerPos"></param>
	/// <param name="cameraFront"></param>
	/// <param name="cameraSpee"></param>
	void ProcessInputs(glm::vec3& camerPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float cameraSpee);

	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);

	static void ScrollCallback(GLFWwindow* window, double offsetX, double offsetY);

	void UpdateMousePosition(float xpos, float ypos);

	void WindowManager::UpdateMouseScroll(float offsetX, float offsetY);

	void CaptureCursor();

	void PollEvents();

	void Shutdown();

	void SetVsync(Vsync interval);

	void PrintHardwareInfo();

	static std::shared_ptr<WindowManager> GetInstance();

	void GetMouseOffsets(float& offsetX, float& offsetY);

	void GetFOV(float &out_FOV);

	// Not the safest behavior here
	GLFWwindow* GetWindow()
	{
		return m_window;
	}

	// Should make this private
	

private:
	static std::shared_ptr<WindowManager> m_instance;
	const std::string resources = "..//resources";
	GLFWwindow* m_window{};
	float m_cursorOffsetX{}, m_cursorOffsetY{};
	bool m_activeMouse{};
	float m_FOV = 90.0f;
	

}; // end class


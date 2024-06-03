#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "string"
#include "manager.hpp"
#include <memory>

enum class Vsync {
	Off = 0,
	On = 1,
	DoubleBuffered = 2,
	TripleBuffered = 3,
};

class WindowManager : public Xplor::Manager<WindowManager>
{

public:
	glm::vec4 m_clear_color{0.1f, 0.3f, 0.5f, 1.0f};


	void init(int windowWidth, int windowHeight, bool fullscreen);

	void updateBuffers();

	/// <summary>
	/// Need to move the camera data out of here. This is a temporary measure
	/// </summary>
	/// <param name="camerPos"></param>
	/// <param name="cameraFront"></param>
	/// <param name="cameraSpee"></param>
	void processInputs(glm::vec3 & out_camera_pos, const glm::vec3 camera_front, const glm::vec3 camera_up, const float camera_speed);

	void setCallbacks();

	static void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);

	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	static void scrollCallback(GLFWwindow* window, double offsetX, double offsetY);

	static void leftMouseClickCallback(GLFWwindow* window, int mods);

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void characterCallback(GLFWwindow* window, unsigned int codepoint);

	void UpdateMousePosition(float xpos, float ypos);

	void WindowManager::UpdateMouseScroll(float offsetX, float offsetY);

	void NewImguiFrame();

	void CreateEditorUI();

	/// <summary>
	/// Set GLFW's capture behavior regarding the cursor
	/// </summary>
	/// <param name="mode">GLFW_CURSOR_NORMAL, GLFW_CURSOR_DISABLED, GLFW_CURSOR_HIDDEN</param>
	void CaptureCursor(int mode);

	void PollEvents();

	void Shutdown();

	void SetVsync(Vsync interval);

	void PrintHardwareInfo();

	void GetMouseOffsets(float& offsetX, float& offsetY);

	void GetFOV(float &out_FOV);

	// Not the safest behavior here
	GLFWwindow* getWindow()
	{
		return m_window;
	}

	// Should make this private
	

private:
	const std::string resources = "..//resources";
	GLFWwindow* m_window{};
	float m_cursorOffsetX{}, m_cursorOffsetY{};
	bool m_activeMouse{};
	float m_FOV = 90.0f;
	

	bool createSelectShader();

}; // end class


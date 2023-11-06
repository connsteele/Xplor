#pragma once

#include <glad/glad.h>
#include "glfw/glfw3.h"
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

	void ProcessEvents();

	void Shutdown();

	void SetVsync(VsyncInterval interval);

	void PrintHardwareInfo();

	// Should make this private
	GLFWwindow* m_window{};

private:
	const std::string resources = "..//resources";
	

}; // end class


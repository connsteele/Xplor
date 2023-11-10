#include "window_manager.hpp"
#include <iostream>

//--------- GLFW Function Prototypes Impls 
//------------------------------------------------------------------------------------------

// Implement the GLFW callback function prototype
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Set the drawing location to the bottom left of the window and set the rendering area
	// This actually performs the transformation of 2D coordinates to screen locations
	glViewport(0, 0, width, height);


	float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

	// Update your projection matrix to match the new aspect ratio
	// For example, if you're using a perspective projection:
	// glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	// glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

//--------- Window Manager Member Variable in-class initializer
std::shared_ptr<WindowManager> WindowManager::m_instance = nullptr;


//--------- Window Manager Methods Impls 
//------------------------------------------------------------------------------------------

void WindowManager::Init(int windowWidth, int windowHeight, bool fullscreen)
{
	//--- Start glfw up
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return;
	}

	//--- Set Window Attributes
	// Set the context to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Set OpenGL to Core-profile mode
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create the window itself
	if (fullscreen)
		m_window = glfwCreateWindow(windowWidth, windowHeight, "XplorEngine", glfwGetPrimaryMonitor(), NULL);
	else
		m_window = glfwCreateWindow(windowWidth, windowHeight, "XplorEngine", NULL, NULL);
	// Setup a user pointer to get data later 
	glfwSetWindowUserPointer(m_window, this);

	// Cleanup GLFW if the window creation fails
	if (!m_window)
	{
		std::cout << "Failed to create a GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_window);

	// Register the frame buffer size callback to upate the viewport when the window
	// changes size
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	// Give GLAD the address of OpenGL function pointers GLFW
	// will give us the right ones for the OS we are using
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// GLAD need to have a context to check the OpenGL version against
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	SetVsync(VsyncInterval::On);
}

// Swaps the front and back buffers
void WindowManager::Update()
{
	glfwSwapBuffers(m_window);
}

void WindowManager::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	WindowManager* windowManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	if (windowManager) {
		windowManager->UpdateMousePosition(static_cast<float>(xpos), static_cast<float>(ypos));
	}
}

void WindowManager::ScrollCallback(GLFWwindow* window, double offsetX, double offsetY)
{
	WindowManager* windowManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	if (windowManager) {
		windowManager->UpdateMouseScroll(static_cast<float>(offsetX), static_cast<float>(offsetY));
	}
}

void WindowManager::UpdateMouseScroll(float offsetX, float offsetY)
{
	m_FOV -= offsetY;

	// Limit the FOV range
	if (m_FOV < 1.0f)
		m_FOV = 1.0f;
	if (m_FOV > 120.0f)
		m_FOV = 120.0f;
}



void WindowManager::UpdateMousePosition(float xpos, float ypos)
{
	m_activeMouse = true;

	static float lastX = xpos;
	static float lastY = ypos;


	m_cursorOffsetX = xpos - lastX;
	m_cursorOffsetY = lastY - ypos; // Y coord starts at bottom
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 2.0f;
	m_cursorOffsetX *= sensitivity;
	m_cursorOffsetY *= sensitivity;
}

void WindowManager::CaptureCursor()
{
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(m_window, WindowManager::MouseCallback);
	glfwSetScrollCallback(m_window, WindowManager::ScrollCallback);
	//glfwSetCursorPos(m_window, 0.0, 0.0);
}

void WindowManager::PollEvents()
{
	glfwPollEvents();
}

// Handle all incoming keyboard and mouse events
void WindowManager::ProcessInputs(glm::vec3& out_cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float cameraSpeed)
{
	if (!m_window)
	{
		std::cout << "Error: Window not created" << std::endl;
		return;
	}

	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// Enable the close window flag
		glfwSetWindowShouldClose(m_window, true);
	}
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		out_cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		out_cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		out_cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		out_cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}
}

void WindowManager::Shutdown()
{
	if (m_window)
	{
		glfwDestroyWindow(m_window);

	}
	glfwTerminate();
}

void WindowManager::SetVsync(VsyncInterval interval)
{
	glfwSwapInterval(static_cast<int>(interval));
}

void WindowManager::PrintHardwareInfo()
{
	auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	auto openglVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	std::cout << "Vendor: " << vendor << "\nRenderer: " << renderer << std::endl;
	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	std::cout << "OpenGL Version: " << major << "." << minor << std::endl;
}


std::shared_ptr<WindowManager> WindowManager::GetInstance()
{
	if (!m_instance)
	{
		m_instance = std::make_shared<WindowManager>();
	}

	return m_instance;
}



void WindowManager::GetMouseOffsets(float&offsetX, float& offsetY)
{
	if (!m_activeMouse)
	{
		m_cursorOffsetX = 0.f;
		m_cursorOffsetY = 0.f;
	}
	offsetX = m_cursorOffsetX;
	offsetY = m_cursorOffsetY;

	// Turn the mouse off for no movement
	m_activeMouse = false;
}

void WindowManager::GetFOV(float& out_FOV)
{
	out_FOV = m_FOV;
}

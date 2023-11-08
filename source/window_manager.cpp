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

// Handle all incoming keyboard and mouse events
void WindowManager::ProcessInputs(glm::vec3& camerPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float cameraSpeed)
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
		camerPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camerPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camerPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camerPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}
}

//--------- Window Manager Methods Impls 
//------------------------------------------------------------------------------------------

void WindowManager::Init()
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
	int windowWidth = 1280;
	int windowHeight = 720;
	m_window = glfwCreateWindow(windowWidth, windowHeight, "XplorEngine", NULL, NULL);

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

void WindowManager::PollEvents()
{
	glfwPollEvents();
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
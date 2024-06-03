#include "window_manager.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "engine_manager.hpp"
#include "shader_manager.hpp"
#include <iostream>

//--------- GLFW Function Prototypes Impls 
//------------------------------------------------------------------------------------------

// Implement the GLFW callback function prototype
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Set the drawing location to the bottom left of the window and set the rendering area
	// This actually performs the transformation of 2D coordinates to screen locations
	glViewport(0, 0, width, height);


	float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);

	// Update your projection matrix to match the new aspect ratio
	// For example, if you're using a perspective projection:
	// glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	// glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

//--------- Window Manager Member Variable in-class initializer
std::shared_ptr<WindowManager> WindowManager::m_instance{ nullptr };


//--------- Window Manager Methods Impls 
//------------------------------------------------------------------------------------------

void WindowManager::init(int window_width, int window_height, bool fullscreen)
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
		m_window = glfwCreateWindow(window_width, window_height, "XplorEngine", glfwGetPrimaryMonitor(), NULL);
	else
		m_window = glfwCreateWindow(window_width, window_height, "XplorEngine", NULL, NULL);
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
		// GLAD needs to have a context to check the OpenGL version against
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// Initial viewport setup
	/*int window_width, window_height;
	glfwGetWindowSize(m_window, &window_width, &window_height);*/
	glViewport(0, 0, window_width, window_height);

	SetVsync(Vsync::On);

	//--------- ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();
	ImFontConfig imConfig;
	imConfig.SizePixels = 18.0f;
	io.Fonts->AddFontDefault(&imConfig);
	// io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	// GL 3.3 + GLSL 330
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);


	// Create the object selection shader
	createSelectShader();
}

// Swaps the front and back buffers
void WindowManager::updateBuffers()
{
	glfwSwapBuffers(m_window);
}

void WindowManager::Shutdown()
{
	if (m_window)
	{
		glfwDestroyWindow(m_window);

	}
	glfwTerminate();
}

void WindowManager::SetVsync(Vsync interval)
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



void WindowManager::GetFOV(float& out_FOV)
{
	out_FOV = m_FOV;
}



//---------- Mouse
//------------------------------------------------------------------------------------------

void WindowManager::MouseMoveCallback(GLFWwindow* window, double x_pos, double y_pos)
{
	WindowManager* windowManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	if (windowManager)
	{
		windowManager->UpdateMousePosition(static_cast<float>(x_pos), static_cast<float>(y_pos));
	}
}

void WindowManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		leftMouseClickCallback(window, mods);
	}
}

void WindowManager::leftMouseClickCallback(GLFWwindow* window, int mods)
{
	constexpr bool CURSOR_RAYCAST = true; // This should probably be a member variable
	constexpr bool DEBUG = false; // Spawn debugging objects where clicked

	if (CURSOR_RAYCAST)
	{
		auto engine_manager = Xplor::EngineManager::getInstance();

		// Clear the selected game objects
		if (mods != GLFW_MOD_SHIFT)
			engine_manager->clearSelection();

		//--- Convert screen coordinates to world coordinates via Ray Casting
		double x_pos, y_pos;
		glfwGetCursorPos(window, &x_pos, &y_pos);

		
		auto camera = engine_manager->getCamera();
		auto view = camera->m_view_matrix;
		auto projection = camera->m_projection_matrix;


		int window_width, window_height;
		glfwGetWindowSize(window, &window_width, &window_height);
		glm::vec4 viewport = glm::vec4(0, 0, window_width, window_height);

		// Invert the Y coordinate
		y_pos = window_height - y_pos;

		// Convert Normalized Device Coordinates (NDC) -> Homogenous Clip Space -> Eye Coordinates -> World Coordinates
		glm::vec3 ray_start = glm::unProject(glm::vec3(x_pos, y_pos, 0.0f), view, projection, viewport);
		glm::vec3 ray_end = glm::unProject(glm::vec3(x_pos, y_pos, 1.0f), view, projection, viewport);
		glm::vec3 ray_direction = glm::normalize(ray_end - ray_start);

		Xplor::Ray cursor_ray{ ray_start, ray_direction, 1.0f / ray_direction };


		if (DEBUG)
		{
			//std::cout << "World coordinates: (" << ray_start.x << ", " << ray_start.y << ", " << ray_start.z << ")" << std::endl;
			// Animate this along the path
			engine_manager->addDebugObject(ray_start, ray_direction);
		}

		//--- Perform Ray Intersections
		engine_manager->rayCursorTest(cursor_ray);
	}
}


void WindowManager::SetMouseCallbacks()
{
	
	glfwSetCursorPosCallback(m_window, WindowManager::MouseMoveCallback); // Creates issues with imgui, also determines if the camera will move with the mouse or not
	glfwSetScrollCallback(m_window, WindowManager::ScrollCallback);
	glfwSetMouseButtonCallback(m_window, WindowManager::MouseButtonCallback);
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


	const float sensitivity = 0.02f;
	m_cursorOffsetX = (xpos - lastX) * sensitivity;
	m_cursorOffsetY = (lastY - ypos) * sensitivity;

	lastX = xpos;
	lastY = ypos;
}

void WindowManager::CaptureCursor(int mode)
{
	glfwSetInputMode(m_window, GLFW_CURSOR, mode);
	//glfwSetCursorPos(m_window, 0.0, 0.0);
}

void WindowManager::GetMouseOffsets(float& offsetX, float& offsetY)
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


//----------- Inputs (general)
//------------------------------------------------------------------------------------------
void WindowManager::PollEvents()
{
	glfwPollEvents();
}

// Handle all incoming keyboard and mouse events
void WindowManager::ProcessInputs(glm::vec3& out_camera_pos, const glm::vec3 camera_front, const glm::vec3 camera_up, const float camera_speed)
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
		out_camera_pos += camera_speed * camera_front;
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		out_camera_pos -= camera_speed * camera_front;
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		out_camera_pos -= camera_speed * glm::normalize(glm::cross(camera_front, camera_up));
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		out_camera_pos += camera_speed * glm::normalize(glm::cross(camera_front, camera_up));
	}
}


//----------- IMGUI
//------------------------------------------------------------------------------------------

void WindowManager::NewImguiFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void WindowManager::CreateEditorUI()
{
	static float f = 0.0f;
	static int counter = 0;


	ImGui::Begin("Editor"); // Create a window and append into it.

	ImGui::Text("UI Text.");
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	ImGui::ColorEdit3("clear color", (float*)&m_clear_color);

	if (ImGui::Button("Button"))   // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);
	auto io = ImGui::GetIO();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();
}

//------------------------------------------------------ Private Methods ------------------------------------------------------//

bool WindowManager::createSelectShader()
{
	auto shader_manager = Xplor::ShaderManager::getInstance();

	std::string vertex = resources + "//shaders//select.vs";
	std::string fragment = resources + "//shaders//select.fs";


	auto shader_info = Xplor::ShaderBasics{"select", vertex, fragment};

	try
	{
		shader_manager->createShader(shader_info);
	}
	catch (std::runtime_error err)
	{
		__debugbreak(); // Windows only
		std::cerr << "Caught runtime error: " << err.what() << std::endl;
	}

	return false;
}

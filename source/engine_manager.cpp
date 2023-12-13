#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "engine_manager.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


std::shared_ptr<Xplor::EngineManager> Xplor::EngineManager::m_instance = nullptr;

void Xplor::EngineManager::Init()
{

}

void Xplor::EngineManager::CreateWindow(int width, int height, bool fullscreen)
{
	std::shared_ptr<WindowManager> windowManager = WindowManager::GetInstance();
	windowManager->Init(width, height, fullscreen);
	// windowManager->CaptureCursor();

	// Register callbacks

}

void Xplor::EngineManager::CreateCamera(CameraVectors vectors, float speed, float fov)
{
	float cameraSpeed = speed;
	float cameraFOV = fov;

	m_activeCamera = std::make_unique<Camera>(vectors, cameraSpeed, cameraFOV);
}

bool Xplor::EngineManager::Run()
{
    auto RebuildFontAtlas = [](float fontSize) {
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->Clear();

        ImFontConfig imConfig;
        imConfig.SizePixels = fontSize;
        io.Fonts->AddFontDefault(&imConfig);
        ImGui_ImplOpenGL3_DestroyFontsTexture();
        ImGui_ImplOpenGL3_CreateFontsTexture();
        };

    float fontSize = 18.0f;
    RebuildFontAtlas(fontSize);

    auto windowManager = WindowManager::GetInstance();
    while (!glfwWindowShouldClose(windowManager->GetWindow())) // Need to setup my own events for this to work better
    {
        //--- Update Delta Time
        float currentFrameTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrameTime - m_lastFrameTime;
        m_lastFrameTime = currentFrameTime;



        //--- Input
        //-----------------------------------------------------
        windowManager->PollEvents();
        //float cameraFinalSpeed = m_activeCamera->m_speed * deltaTime;

        // I need to change how this logic happens. The window manager should process the inputs here
        // but then after that it should let the camera know if it needs to move
        //windowManager->ProcessInputs(cameraPosition, cameraFront, cameraUp, cameraFinalSpeed);

        //--- ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static float f = 0.0f;
        static int counter = 0;
        static bool show_demo_window = true;
        static bool show_another_window = false;
        if (show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);

        }

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        static glm::vec4 clear_color(0.1f, 0.3f, 0.5f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        auto io = ImGui::GetIO();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering commands
        //-----------------------------------------------------

        //---- Background Color
        glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
        glClearColor(clear_color.x * clear_color.w,
            clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        //---- Logic Commands
        static bool move = true;
        /*if (move && m_gameObjects[1])
        {
            move = false;
            m_gameObjects[1]->AddImpulse(glm::vec3(-1.0, 0.0, 0.0));
        }
        static bool rotate = true;
        if (rotate && m_gameObjects[2])
        {
            rotate = false;
            m_gameObjects[2]->SetRotation(glm::vec3(1.0f, 0.0f, 0.0f), 45.f);
        }*/

        //--- Logic Update
        Update(deltaTime);

        //---- Scene Rendering
        Render(m_activeCamera->m_viewMatrix, m_activeCamera->m_projectionMatrix);

        //---- ImGui Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(windowManager->GetWindow(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        // Swap the front and back buffers
        windowManager->Update();

        // Check for window font resizing
        /*fontSize = 20;
        RebuildFontAtlas(fontSize);*/
    }

	return false;
}


void Xplor::EngineManager::Update(float deltaTime)
{
    m_activeCamera->Update(deltaTime);

    for (auto object : m_gameObjects)
    {
        object->Update(deltaTime);
    }

}

void Xplor::EngineManager::Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	for (auto object : m_gameObjects)
	{
		object->Render(viewMatrix, projectionMatrix);
	}
}

std::shared_ptr<Xplor::EngineManager> Xplor::EngineManager::GetInstance()
{
	if (!m_instance)
	{
		m_instance = std::make_shared<Xplor::EngineManager>();
	}

	return m_instance;
}

void Xplor::EngineManager::AddGameObject(std::shared_ptr<GameObject> object)
{
    object->SetID(++m_objectCount);
	m_gameObjects.push_back(object);
}

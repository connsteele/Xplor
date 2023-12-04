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
	windowManager->CaptureCursor();

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
    auto windowManager = WindowManager::GetInstance();

    while (!glfwWindowShouldClose(windowManager->GetWindow())) // Need to setup my own events for this to work better
    {
        //--- Update Delta Time
        float currentFrameTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrameTime - m_lastFrameTime;
        m_lastFrameTime = currentFrameTime;

        //--- Input
        //-----------------------------------------------------

        float cameraFinalSpeed = m_activeCamera->m_speed * deltaTime;
        // I need to change how this logic happens. The window manager should process the inputs here
        // but then after that it should let the camera know if it needs to move
        //windowManager->ProcessInputs(cameraPosition, cameraFront, cameraUp, cameraFinalSpeed);

        // Rendering commands
        //-----------------------------------------------------

        //---- Background Color
        glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


        //--- Logic Update
        Update(deltaTime);

        //---- Rendering
        Render(m_activeCamera->m_viewMatrix, m_activeCamera->m_projectionMatrix);

        // Swap the front and back buffers
        windowManager->Update();
        windowManager->PollEvents();
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

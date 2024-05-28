#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "engine_manager.hpp"
#include "generator_geometry.hpp"
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

    // ------- Set mouse related callbacks and window functionality
	
    windowManager->SetMouseCallbacks();
	windowManager->CaptureCursor(GLFW_CURSOR_NORMAL);

}

void Xplor::EngineManager::CreateCamera(CameraVectors vectors, float speed, float fov)
{
	float cameraSpeed = speed;
	float cameraFOV = fov;

	m_active_camera = std::make_shared<Camera>(vectors, cameraSpeed, cameraFOV);
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

    auto window_manager = WindowManager::GetInstance();
    while (!glfwWindowShouldClose(window_manager->GetWindow())) // Need to setup my own events for this to work better
    {
        //--- Update Delta Time
        float current_frame_time = static_cast<float>(glfwGetTime());
        float delta_time = current_frame_time - m_last_frame_time;
        m_delta_time = delta_time;
        m_last_frame_time = current_frame_time;


        //--- Input
        //-----------------------------------------------------
        window_manager->PollEvents();
        //float cameraFinalSpeed = m_activeCamera->m_speed * deltaTime;

        // I need to change how this logic happens. The window manager should process the inputs here
        // but then after that it should let the camera know if it needs to move
        //windowManager->ProcessInputs(cameraPosition, cameraFront, cameraUp, cameraFinalSpeed);

        //--- ImGui
        window_manager->NewImguiFrame();

        window_manager->CreateEditorUI();

        // Rendering commands
        //-----------------------------------------------------

        //---- Background Color
        glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
        glClearColor(window_manager->m_clear_color.x * window_manager->m_clear_color.w,
            window_manager->m_clear_color.y * window_manager->m_clear_color.w,
            window_manager->m_clear_color.z * window_manager->m_clear_color.w,
            window_manager->m_clear_color.w);
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
        Update(delta_time);

        //---- Scene Rendering
        Render(m_active_camera->m_view_matrix, m_active_camera->m_projection_matrix);

        //---- ImGui Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window_manager->GetWindow(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        // Swap the front and back buffers
        window_manager->UpdateBuffers();

        // Check for window font resizing
        /*fontSize = 20;
        RebuildFontAtlas(fontSize);*/
    }

	return false;
}


void Xplor::EngineManager::Update(float deltaTime)
{
    m_active_camera->Update(deltaTime);

    for (auto object : m_gameObjects)
    {               
        object->Update(deltaTime);
    }

}

void Xplor::EngineManager::Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    constexpr bool DEBUG = false;
	for (auto object : m_gameObjects)
	{
		object->Render(viewMatrix, projectionMatrix);
        if (DEBUG)
            object->DrawBoundingBox(viewMatrix, projectionMatrix);
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

void Xplor::EngineManager::RayIntersectionTest(const glm::vec3 ray_start, const glm::vec3 ray_direction)
{
    float t; // depth
    float closest_t = std::numeric_limits<float>::max();
    std::shared_ptr<GameObject> closest_object = nullptr;

    for (const std::shared_ptr<GameObject>& object : m_gameObjects)
    {
        if (RayIntersectsAABB(ray_start, ray_direction, object->GetBoundingBox(), t))
        {
            std::cout << "Ray intersects object: " << object->GetName() << " at t = " << t << std::endl;
            if (t < closest_t)
            {
                closest_t = t;
                closest_object = object;
            }
            // Add visual feedback, e.g., change object color
            //object->SetColor(glm::vec3(1.0f, 0.0f, 0.0f)); // Assuming you have a SetColor method
        }
    }

    if (closest_object)
    {
        std::cout << "Ray intersected the closest object: " << closest_object->GetName() << std::endl;
    }
}

/// <summary>
/// Check for the closest Bounding Box that is closest to the rays origin in a given direction
/// </summary>
/// <param name="ray_origin">Converted world coordinate of where cursor is clicked</param>
/// <param name="ray_direction">The direction from the ray's start to where it terminates</param>
/// <param name="bbox">Bounding box of a game object</param>
/// <param name="out_t">Indicates the distance from the ray's origin to the intersection point along the ray's direction vector</param>
/// <returns></returns>
bool Xplor::EngineManager::RayIntersectsAABB(const glm::vec3& ray_origin, const glm::vec3& ray_direction, const BoundingBox& bbox, float& out_t)
{
    // Ray intersections with X axis
    float t_min = (bbox.min.x - ray_origin.x) / ray_direction.x; // entry point
    float t_max = (bbox.max.x - ray_origin.x) / ray_direction.x; // exit point

    // Ensure the entry and exit points are in proper order
    if (t_min > t_max)
        std::swap(t_min, t_max);
    

    // Ray intersections with Y axis
    float ty_min = (bbox.min.y - ray_origin.y) / ray_direction.y;
    float ty_max = (bbox.max.y - ray_origin.y) / ray_direction.y;

    // Check if there is no overlap between the intervals on the X and Y axis
    if (t_max < ty_min || t_min > ty_max)
    {
        // Ray misses the bounding box
        return false;
    }

    // Update the intersection interval that accounts for the X and Y planes
    t_min = std::max(t_min, ty_min); // update entry point to its latest intersection
    t_max = std::min(t_max, ty_max); // update exit point to its earliest exit
        

    // Ray intersections with the Z axis
    float tz_min = (bbox.min.z - ray_origin.z) / ray_direction.z;
    float tz_max = (bbox.max.z - ray_origin.z) / ray_direction.z;

    // Check if there is no overlap between the intervals on the X, Y and Z axis
    if (t_max  < tz_min || t_min > tz_max)
    {
        // Ray misses the bounding box
        return false;
    }

    // Update the intersection interval that accounts for the X, Y and Z planes
    t_min = std::max(t_min, tz_min); // update entry point to its latest intersection
    t_max = std::min(t_max, tz_max); // update exit point to its earliest exit


    out_t = t_min; //Smallest depth where the ray intersects the box
    return true; // Ray intersects bbox
}

/// <summary>
/// Create a cube prop with a debug texture at the give position
/// </summary>
/// <param name="position"></param>
void Xplor::EngineManager::AddDebugObject(const glm::vec3& position)
{
    std::shared_ptr<Xplor::PropObject> debug_object = std::make_shared<Xplor::PropObject>();
    debug_object->SetName("Debug Object");
    debug_object->SetPosition(position);

    debug_object->AddTexture("images//debug.jpg", ImageFormat::jpg);
    debug_object->InitTextures();

    // Add a simple shader
    auto shader = std::make_shared<Shader>("..//resources//shaders//simple.vs", "..//resources//shaders//simple.fs");
    shader->Init();
    debug_object->AddShader(shader);
    auto shader_id = debug_object->GetShader();

    shader_id->useProgram();
    shader_id->setUniform("customTexture1", 0);
    shader_id->endProgram();

    auto cube_data = GeometryGenerator::GenerateCubeData();
    const int step_size = 5;
    const int index_count = 36;
    debug_object->AddGeometry(cube_data.data(), cube_data.size(), step_size, index_count);
    debug_object->InitGeometry();


    AddGameObject(debug_object);

}

/// <summary>
/// Create a cube prop with a debug texture at a given position with a velocity
/// </summary>
/// <param name="position"></param>
/// <param name="velocity"></param>
void Xplor::EngineManager::AddDebugObject(const glm::vec3& position, const glm::vec3& velocity)
{
    std::shared_ptr<Xplor::PropObject> debug_object = std::make_shared<Xplor::PropObject>();
    debug_object->SetName("Debug Object");
    debug_object->SetPosition(position);

    debug_object->AddTexture("images//debug.jpg", ImageFormat::jpg);
    debug_object->InitTextures();

    // Add a simple shader
    auto shader = std::make_shared<Shader>("..//resources//shaders//simple.vs", "..//resources//shaders//simple.fs");
    shader->Init();
    debug_object->AddShader(shader);
    auto shader_id = debug_object->GetShader();

    shader_id->useProgram();
    shader_id->setUniform("customTexture1", 0);
    shader_id->endProgram();

    auto cube_data = GeometryGenerator::GenerateCubeData();
    const int step_size = 5;
    const int index_count = 36;
    debug_object->AddGeometry(cube_data.data(), cube_data.size(), step_size, index_count);
    debug_object->InitGeometry();


    debug_object->SetVelocity(velocity);
    AddGameObject(debug_object);

}
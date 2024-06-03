#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "engine_manager.hpp"
#include "generator_geometry.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_manager.hpp>


void Xplor::EngineManager::Init()
{

}

void Xplor::EngineManager::createWindow(int width, int height, bool fullscreen)
{
	std::shared_ptr<WindowManager> windowManager = WindowManager::getInstance();
    windowManager->init(width, height, fullscreen);

    // ------- Set mouse related callbacks and window functionality
	
    windowManager->setCallbacks();
	windowManager->CaptureCursor(GLFW_CURSOR_NORMAL);

}

void Xplor::EngineManager::createCamera(CameraVectors vectors, float speed, float fov)
{
	float cameraSpeed = speed;
	float cameraFOV = fov;

	m_active_camera = std::make_shared<Camera>(vectors, cameraSpeed, cameraFOV);
}

bool Xplor::EngineManager::run()
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

    auto window_manager = WindowManager::getInstance();
    while (!glfwWindowShouldClose(window_manager->getWindow())) // Need to setup my own events for this to work better
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
        update(delta_time);

        //---- Scene Rendering
        render(m_active_camera->m_view_matrix, m_active_camera->m_projection_matrix);

        //---- ImGui Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window_manager->getWindow(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        // Swap the front and back buffers
        window_manager->updateBuffers();

        // Check for window font resizing
        /*fontSize = 20;
        RebuildFontAtlas(fontSize);*/
    }

	return false;
}


void Xplor::EngineManager::update(float delta_time)
{
    m_active_camera->Update(delta_time);

    for (auto object : m_game_objects)
    {               
        object->update(delta_time);
    }

}

void Xplor::EngineManager::render(glm::mat4 view_matrix, glm::mat4 projection_matrix)
{
    constexpr bool DRAW_BOUNDING = true;
    

	for (auto object : m_game_objects)
	{
		object->draw(view_matrix, projection_matrix);
        

        if (auto search = m_selected.find(object) ; search != m_selected.end())
        {
            // Draw the selection overlay
            glEnable(GL_BLEND); // Allow alpha
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_DEPTH_TEST); // Disable depth testing to draw ontop

            object->draw(view_matrix, projection_matrix, "select");

            glEnable(GL_DEPTH_TEST);
            glDisable(GL_BLEND);
        }

        if (DRAW_BOUNDING)
        {
            object->drawBoundingBox(view_matrix, projection_matrix);
        }   
	}    
}

void Xplor::EngineManager::addGameObject(std::shared_ptr<GameObject> object)
{
    object->setID(++m_object_count);
	m_game_objects.push_back(object);
}

void Xplor::EngineManager::rayCursorTest(const Xplor::Ray& ray)
{
    float t; // depth
    float closest_t = std::numeric_limits<float>::max();
    std::shared_ptr<GameObject> closest_object = nullptr;

    for (const std::shared_ptr<GameObject>& object : m_game_objects)
    {
        if (rayIntersect(ray, object->getBoundingBox(), t))
        {
            std::cout << "Ray intersects object: " << object->getName() << " at t = " << t << std::endl;
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
        m_selected.insert(closest_object);
        std::cout << "Ray intersected the closest object: " << closest_object->getName() << std::endl;
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
bool Xplor::EngineManager::rayIntersect(const Xplor::Ray & ray, const BoundingBox& bbox, float& out_t)
{
    float tmin = std::numeric_limits<float>::min();
    float tmax = std::numeric_limits<float>::max();

    // Go through each dimension (x, y, z)
    for (int d = 0; d < 3; d++)
    {
        float t1 = (bbox.min[d] - ray.origin[d]) * ray.direction_inv[d];
        float t2 = (bbox.max[d] - ray.origin[d]) * ray.direction_inv[d];

        tmin = std::min(std::max(t1, tmin), std::max(t2, tmin));
        tmax = std::max(std::min(t1, tmax), std::min(t2, tmax));

    }

    out_t = (tmin <= tmax) ? tmin : -1.0f;

    return tmin <= tmax;
}

/// <summary>
/// Create a cube prop with a debug texture at the give position
/// </summary>
/// <param name="position"></param>
void Xplor::EngineManager::addDebugObject(const glm::vec3& position)
{
    std::shared_ptr<Xplor::PropObject> debug_object = std::make_shared<Xplor::PropObject>();
    debug_object->setName("Debug Object");
    debug_object->setPosition(position);

    debug_object->addTexture("images//debug.jpg", ImageFormat::jpg);
    debug_object->initTextures();

    // Add a simple shader
    auto shader = std::make_shared<Shader>("..//resources//shaders//simple.vs", "..//resources//shaders//simple.fs");
    shader->init();
    debug_object->addShader(shader);
    auto shader_id = debug_object->getShader();

    shader_id->useProgram();
    shader_id->setUniform("customTexture1", 0);
    shader_id->endProgram();

    auto cube_data = GeometryGenerator::GenerateCubeData();
    const int step_size = 5;
    const int index_count = 36;
    debug_object->addGeometry(cube_data.data(), cube_data.size(), step_size, index_count);
    debug_object->initGeometry();


    addGameObject(debug_object);

}

/// <summary>
/// Create a cube prop with a debug texture at a given position with a velocity
/// </summary>
/// <param name="position"></param>
/// <param name="velocity"></param>
void Xplor::EngineManager::addDebugObject(const glm::vec3& position, const glm::vec3& velocity)
{
    std::shared_ptr<Xplor::PropObject> debug_object = std::make_shared<Xplor::PropObject>();
    debug_object->setName("Debug Object");
    debug_object->setPosition(position);

    // Should have a way to check if the current texture already exists
    // this should exist somewhere else for better performance
    debug_object->addTexture("images//debug.jpg", ImageFormat::jpg);
    debug_object->initTextures();

    // Use a the one texture shader
    auto shader_manager = ShaderManager::getInstance();
    std::shared_ptr<Shader> shader;
    shader_manager->findShader("one texture", shader);
    debug_object->addShader(shader);

    auto cube_data = GeometryGenerator::GenerateCubeData();
    const int step_size = 5;
    const int index_count = 36;
    debug_object->addGeometry(cube_data.data(), cube_data.size(), step_size, index_count);
    debug_object->initGeometry();

    debug_object->setVelocity(velocity);
    addGameObject(debug_object);

}

void Xplor::EngineManager::clearSelection()
{
    m_selected.clear();
}


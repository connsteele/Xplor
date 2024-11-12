
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "shader.hpp"
#include "engine_manager.hpp"
#include "window_manager.hpp"
#include "game_object.hpp"
#include "camera.hpp"
#include "generator_geometry.hpp"
#include "shader_manager.hpp"

struct ImgData
{
    int width;
    int height;
    int channels;
    unsigned char* data;
};


void printMaxVertexAttrib()
{
    int maxAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
    std::cout << "Maxmimum number of vertex attributes supported: " << maxAttributes << std::endl;

}

void createSceneA()
{
    //------ Object Creation
    std::shared_ptr<Xplor::PropObject> planeA = std::make_shared<Xplor::PropObject>();
    std::shared_ptr<Xplor::PropObject> cubeA = std::make_shared<Xplor::PropObject>();
    std::shared_ptr<Xplor::PropObject> cubeB = std::make_shared<Xplor::PropObject>();

    planeA->setName("Metal Plane");
    cubeA->setName("Box Dog");
    cubeB->setName("Box Metal");

    //------ Image Loading
    planeA->addTexture("images//Firefly_Metal_Box.jpg", Xplor::ImageFormat::jpg);
    planeA->initTextures();

    cubeA->addTexture("images//woodBox.jpg", Xplor::ImageFormat::jpg);
    cubeA->addTexture("images//dog.png", Xplor::ImageFormat::png);
    cubeA->initTextures();


    cubeB->addTexture("images//Firefly_Metal_Box.jpg", Xplor::ImageFormat::jpg);
    cubeB->addTexture("images//dog.png", Xplor::ImageFormat::png);
    cubeB->initTextures();


    //------ Shader Creation
    const std::string resources{ "..//resources" };
    std::string full_vertex_path{ resources + "//shaders//simple.vs" };
    std::string full_frag_path{ resources + "//shaders//simple.fs" };
    std::string full_frag_one_tex_path{ resources + "//shaders//simpleOneTex.fs" };
    std::string full_frag_flat_color_path{ resources + "//shaders//flatColor.fs" };
    std::string bbox_vertex_full_path{ resources + "//shaders//bounding.vs" };
    std::string bbox_fragment_full_path{ resources + "//shaders//bounding_color.fs" };
    std::vector<Xplor::ShaderInfo> shader_paths { 
        {"simple", full_vertex_path, full_frag_path},
        {"one texture", full_vertex_path, full_frag_one_tex_path}, 
        {"flat color", full_vertex_path, full_frag_flat_color_path},
        {"bounding", bbox_vertex_full_path, bbox_fragment_full_path}
    };

    auto shader_manager = Xplor::ShaderManager::getInstance();
    for (auto shader_info : shader_paths)
    {
        try
        {
            shader_manager->createShader(shader_info);
        }
        catch (const std::runtime_error& error)
        {
            __debugbreak(); // Windows only
            std::cerr << "Caught runtime error: " << error.what() << std::endl;
        }
    }

    std::shared_ptr<Xplor::Shader> shader_one_tex;
    shader_manager->findShader("one texture", shader_one_tex);
    std::shared_ptr<Xplor::Shader> shader_simple;
    shader_manager->findShader("simple", shader_simple);

    planeA->addShader(shader_one_tex);
    cubeA->addShader(shader_simple);
    cubeB->addShader(shader_one_tex);

    // cubeBBOX

    //------ Define shader uniforms
    shader_one_tex->useProgram();
    shader_one_tex->setUniform("customTexture1", 0);
    shader_one_tex->endProgram();

    shader_simple->useProgram();
    // Inform the shader where the texture samplers are located
    shader_simple->setUniform("customTexture1", 0);
    shader_simple->setUniform("customTexture2", 1);
    shader_simple->endProgram();

    auto geometryPlane = GeometryGenerator::GeneratePlaneData();
    auto planeEBO = GeometryGenerator::GeneratePlaneEBO();

    // Send Geometry information to the game object
    planeA->setPosition(glm::vec3(-1.f, 0.0f, 1.f));
    planeA->addGeometry(geometryPlane.data(), geometryPlane.size(), planeEBO.data(), planeEBO.size(), 5);
    planeA->initGeometry();

    auto geometryCube = GeometryGenerator::generateCubeData();
    cubeA->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    cubeA->addGeometry(geometryCube.data(), geometryCube.size(), 5, 36);
    cubeA->initGeometry();

    cubeB->setPosition(glm::vec3(5.0f, 3.0f, -5.0f));
    cubeB->addGeometry(geometryCube.data(), geometryCube.size(), 5, 36);
    cubeB->initGeometry();

    // Temporary - Update BBoxes manually to force accurate generation
    planeA->updateBoundingBox();
    cubeA->updateBoundingBox();
    cubeB->updateBoundingBox();


    std::shared_ptr<Xplor::EngineManager> xplor_manager = Xplor::EngineManager::getInstance();
    // Should create a moveable version of this also (perf benefits of avoiding a copy).
    //xplor_manager->addGameObject(planeA);
    xplor_manager->addGameObject(std::move(planeA));
    xplor_manager->addGameObject(std::move(cubeA));
    xplor_manager->addGameObject(std::move(cubeB));
}

int main(/*int argc, char **argv*/) {
   
    //---- Setup ----
    std::shared_ptr<Xplor::EngineManager> xplor_manager = Xplor::EngineManager::getInstance();

    xplor_manager->createWindow(1920, 1080, false);
    std::shared_ptr<WindowManager> windowManager = WindowManager::getInstance();
    glEnable(GL_DEPTH_TEST);

    windowManager->printHardwareInfo();
    printMaxVertexAttrib();


    //---- Scene Setup
    constexpr bool EXPORT_SCENE = true;
    constexpr bool IMPORT_SCENE = false;
    if (IMPORT_SCENE)
        xplor_manager->importScene("test.json");
    else
        createSceneA();
        

    //---- Camera Setup
    glm::vec3 cam_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cam_front  = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cam_up = glm::vec3(0.f , 1.f, 0.f);
    Xplor::CameraVectors camVecs(
        std::move(cam_pos), 
        std::move(cam_front),
        std::move(cam_up));
    // float cameraSpeed = 3.f;

    xplor_manager->createCamera(camVecs);


    //---- Engine Main Loop ----
    //-----------------------------------------------------
    xplor_manager->run();

    if (EXPORT_SCENE)
        xplor_manager->exportScene("test.json");

    
    //---- Cleanup ----
    //-----------------------------------------------------
    //cubeA->Delete();

    return 0;
}
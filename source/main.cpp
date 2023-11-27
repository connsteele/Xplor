#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
// #include "transform_component.hpp"
#include "shader.hpp"
#include "engine_manager.hpp"
#include "window_manager.hpp"
#include "game_object.hpp"
#include "camera.hpp"

struct ImgData
{
    int width;
    int height;
    int channels;
    unsigned char* data;
};



int main(int argc, char **argv) {
   
    //---- Setup ----
    std::shared_ptr<Xplor::EngineManager> xplorM = Xplor::EngineManager::GetInstance();

    xplorM->CreateWindow(1920, 1080, false);

    std::shared_ptr<WindowManager> windowManager = WindowManager::GetInstance();

    std::shared_ptr<Xplor::PropObject> cube = std::make_shared<Xplor::PropObject>();
    xplorM->AddGameObject(cube);


    //--- Image Loading
    cube->AddTexture("//images//woodBox.jpg", Xplor::ImageFormat::jpg);
    cube->AddTexture("//images//dog.png", Xplor::ImageFormat::png);


    //--- Shader Creation
    //----------------------------------------
    const std::string resources = "..//resources";
    std::string vertexShaderPath = "//shaders//simple.vs";
    std::string fragmentShaderPath = "//shaders//simple.fs";

    //cube->AddShader(vertexShaderPath, fragmentShaderPath);
    std::string fullVertexPath = resources + vertexShaderPath;
    std::string fullFragmentPath = resources + fragmentShaderPath;


    std::shared_ptr<Xplor::Shader> simpleShader = 
        std::make_shared<Xplor::Shader>(Xplor::Shader(fullVertexPath.c_str(), 
            fullFragmentPath.c_str()));

    cube->AddShader(simpleShader);
    auto cubeShader = cube->GetShader();


    //auto cubeShader = cube->GetShader();
    cubeShader->useProgram();
    // Inform the shader where the texture samplers are located
    cubeShader->setUniform("customTexture1", 0);
    cubeShader->setUniform("customTexture2", 1);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    cubeShader->endProgram();


    // Vertex Buffer Setup
    // Rectangle
    size_t verticesCubeSize = 180;
    float verticesCube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // Cubes
    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // Send Geometry information to the game object
    cube->AddGeometry(verticesCube, verticesCubeSize, 5);
    cube->InitGeom();

    // Query hardware information
    windowManager->PrintHardwareInfo();
    // Query the max amount of vertex attribs we can use
    int maxAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
    std::cout << "Maxmimum number of vertex attributes supported: " 
        << maxAttributes << std::endl;


    //---- Camera Setup
     // View space formation
    Xplor::CameraVectors camVecs;
    camVecs.cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    camVecs.cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    camVecs.cameraUp = glm::vec3(0.f , 1.f, 0.f);
    float cameraSpeed = 3.f;
    float fov = 90.f;

    Xplor::Camera sceneCamera(camVecs, cameraSpeed, fov);
    xplorM->CreateCamera(camVecs);


    //---- Engine Main Loop ----
    //-----------------------------------------------------
    xplorM->Run();

    
    //---- Cleanup ----
    //-----------------------------------------------------
    cube->Delete();

    return 0;
}
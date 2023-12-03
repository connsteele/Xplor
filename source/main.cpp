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


void PrintMaxVertexAttrib()
{
    int maxAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
    std::cout << "Maxmimum number of vertex attributes supported: " << maxAttributes << std::endl;

}

int main(int argc, char **argv) {
   
    //---- Setup ----
    std::shared_ptr<Xplor::EngineManager> xplorM = Xplor::EngineManager::GetInstance();

    xplorM->CreateWindow(1920, 1080, false);

    std::shared_ptr<WindowManager> windowManager = WindowManager::GetInstance();

    std::shared_ptr<Xplor::PropObject> cubeA = std::make_shared<Xplor::PropObject>();
    xplorM->AddGameObject(cubeA);

    std::shared_ptr<Xplor::PropObject> cubeB = std::make_shared<Xplor::PropObject>();
    xplorM->AddGameObject(cubeB);


    //--- Image Loading
    cubeA->AddTexture("images//woodBox.jpg", Xplor::ImageFormat::jpg);
    cubeA->AddTexture("images//dog.png", Xplor::ImageFormat::png);

    cubeB->AddTexture("images//Firefly_Metal_Box.jpg", Xplor::ImageFormat::jpg);
    cubeB->AddTexture("images//dog.png", Xplor::ImageFormat::png);


    //--- Shader Creation
    //----------------------------------------
    const std::string resources = "..//resources";
    std::string vertexShaderPath = "//shaders//simple.vs";
    std::string fragmentShaderPath = "//shaders//simple.fs";

    std::string fullVertexPath = resources + vertexShaderPath;
    std::string fullFragmentPath = resources + fragmentShaderPath;


    std::shared_ptr<Xplor::Shader> simpleShader = std::make_shared<Xplor::Shader>
        (Xplor::Shader(fullVertexPath.c_str(), fullFragmentPath.c_str()));

    cubeA->AddShader(simpleShader);
    auto cubeAShader = cubeA->GetShader();
    cubeB->AddShader(simpleShader);
    auto cubeBShader = cubeB->GetShader();

    cubeAShader->useProgram();
    // Inform the shader where the texture samplers are located
    cubeAShader->setUniform("customTexture1", 0);
    cubeAShader->setUniform("customTexture2", 1);
    cubeAShader->endProgram();

    cubeBShader->useProgram();
    cubeBShader->setUniform("customTexture1", 0);
    cubeBShader->setUniform("customTexture2", 1);
    cubeBShader->endProgram();


    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Vertex Buffer Setup
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

    // Send Geometry information to the game object
    cubeA->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    cubeA->AddGeometry(verticesCube, verticesCubeSize, 5);
    cubeA->InitGeom();

    cubeB->SetPosition(glm::vec3(2.0f, 0.0f, -3.0f));
    cubeB->AddGeometry(verticesCube, verticesCubeSize, 5);
    cubeB->InitGeom();


    windowManager->PrintHardwareInfo();
    PrintMaxVertexAttrib();


    //---- Camera Setup
    Xplor::CameraVectors camVecs;
    camVecs.cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    camVecs.cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    camVecs.cameraUp = glm::vec3(0.f , 1.f, 0.f);
    float cameraSpeed = 3.f;

    xplorM->CreateCamera(camVecs);


    //---- Engine Main Loop ----
    //-----------------------------------------------------
    xplorM->Run();

    
    //---- Cleanup ----
    //-----------------------------------------------------
    cubeA->Delete();

    return 0;
}
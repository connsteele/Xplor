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

    std::shared_ptr<Xplor::PropObject> planeA = std::make_shared<Xplor::PropObject>();
    xplorM->AddGameObject(planeA);

    std::shared_ptr<Xplor::PropObject> cubeA = std::make_shared<Xplor::PropObject>();
    xplorM->AddGameObject(cubeA);

    std::shared_ptr<Xplor::PropObject> cubeB = std::make_shared<Xplor::PropObject>();
    xplorM->AddGameObject(cubeB);


    //--- Image Loading
    planeA->AddTexture("images//Firefly_Metal_Box.jpg", Xplor::ImageFormat::jpg);

    cubeA->AddTexture("images//woodBox.jpg", Xplor::ImageFormat::jpg);
    cubeA->AddTexture("images//dog.png", Xplor::ImageFormat::png);

    cubeB->AddTexture("images//Firefly_Metal_Box.jpg", Xplor::ImageFormat::jpg);
    cubeB->AddTexture("images//dog.png", Xplor::ImageFormat::png);


    //--- Shader Creation
    //----------------------------------------
    const std::string resources = "..//resources";
    std::string vertexShaderPath = "//shaders//simple.vs";
    std::string fragmentShaderPath = "//shaders//simple.fs";
    std::string fragmentOneTexShaderPath = "//shaders//simpleOneTex.fs";
    std::string fragFlatColorPath = "//shaders//flatColor.fs";

    std::string fullVertexPath = resources + vertexShaderPath;
    std::string fullFragmentPath = resources + fragmentShaderPath;
    std::string fullFragOneTexPath = resources + fragmentOneTexShaderPath;
    std::string fullFragFlatColorPath = resources + fragFlatColorPath;


    std::shared_ptr<Xplor::Shader> simpleShader = std::make_shared<Xplor::Shader>
        (Xplor::Shader(fullVertexPath.c_str(), fullFragmentPath.c_str()));
    std::shared_ptr<Xplor::Shader> simpleShaderOneTex = std::make_shared<Xplor::Shader>
        (Xplor::Shader(fullVertexPath.c_str(), fullFragOneTexPath.c_str()));
    std::shared_ptr<Xplor::Shader> shaderFlatColor = std::make_shared<Xplor::Shader>
        (Xplor::Shader(fullVertexPath.c_str(), fullFragFlatColorPath.c_str()));


    planeA->AddShader(simpleShaderOneTex);
    auto planeAShader = planeA->GetShader();

    cubeA->AddShader(simpleShader);
    auto cubeAShader = cubeA->GetShader();

    cubeB->AddShader(simpleShaderOneTex);
    auto cubeBShader = cubeB->GetShader();

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    planeAShader->useProgram();
    planeAShader->setUniform("customTexture1", 0);
    planeAShader->endProgram();

    cubeAShader->useProgram();
    // Inform the shader where the texture samplers are located
    cubeAShader->setUniform("customTexture1", 0);
    cubeAShader->setUniform("customTexture2", 1);
    cubeAShader->endProgram();

    cubeBShader->useProgram();
    cubeBShader->setUniform("customTexture1", 0);
    cubeBShader->endProgram();


    

    // Vertex Buffer Setup
    size_t verticesCubeSize = 180;
    // First 3 floats are positins, second two are for textures
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
    size_t verticesPlaneSize = 20;
    // First 3 are vert pos, next 2 are texture coords
    float verticesPlane[] = {
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f, 1.f, 1.f, // bot right
        -0.5f, -0.5f, 0.0f, 1.f, 0.f, // bot left
        -0.5f, 0.5f, -0.0f, 0.f, 0.f // top left
        
    };
    size_t eboSize = 6;
    unsigned int eboPlane[] = {
        0, 1, 3,
        1, 2, 3
    };

    // Send Geometry information to the game object
    planeA->SetPosition(glm::vec3(-1.f, 0.0f, 1.f));
    planeA->AddGeometry(verticesPlane, verticesPlaneSize, eboPlane, eboSize, 5);

    cubeA->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    cubeA->AddGeometry(verticesCube, verticesCubeSize, 5, 36);

    cubeB->SetPosition(glm::vec3(2.0f, 0.0f, -3.0f));
    cubeB->AddGeometry(verticesCube, verticesCubeSize, 5, 36);


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

    xplorM->ExportScene("test.json");

    
    //---- Cleanup ----
    //-----------------------------------------------------
    cubeA->Delete();

    return 0;
}
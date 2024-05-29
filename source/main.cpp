
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include "shader.hpp"
#include "engine_manager.hpp"
#include "window_manager.hpp"
#include "game_object.hpp"
#include "camera.hpp"
#include "generator_geometry.hpp"

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

void CreateSceneA()
{
    //------ Object Creation
    std::shared_ptr<Xplor::PropObject> planeA = std::make_shared<Xplor::PropObject>();
    std::shared_ptr<Xplor::PropObject> cubeA = std::make_shared<Xplor::PropObject>();
    std::shared_ptr<Xplor::PropObject> cubeB = std::make_shared<Xplor::PropObject>();

    planeA->SetName("Metal Plane");
    cubeA->SetName("Box Dog");
    cubeB->SetName("Box Metal");

    //------ Image Loading
    planeA->AddTexture("images//Firefly_Metal_Box.jpg", Xplor::ImageFormat::jpg);
    planeA->InitTextures();

    cubeA->AddTexture("images//woodBox.jpg", Xplor::ImageFormat::jpg);
    cubeA->AddTexture("images//dog.png", Xplor::ImageFormat::png);
    cubeA->InitTextures();


    cubeB->AddTexture("images//Firefly_Metal_Box.jpg", Xplor::ImageFormat::jpg);
    cubeB->AddTexture("images//dog.png", Xplor::ImageFormat::png);
    cubeB->InitTextures();


    //------ Shader Creation
    const std::string resources = "..//resources";
    std::string vertexShaderPath = "//shaders//simple.vs";
    std::string fragmentShaderPath = "//shaders//simple.fs";
    std::string fragmentOneTexShaderPath = "//shaders//simpleOneTex.fs";
    std::string fragFlatColorPath = "//shaders//flatColor.fs";
    

    std::string fullVertexPath = resources + vertexShaderPath;
    std::string fullFragmentPath = resources + fragmentShaderPath;
    std::string fullFragOneTexPath = resources + fragmentOneTexShaderPath;
    std::string fullFragFlatColorPath = resources + fragFlatColorPath;
    std::string bbox_vertex_full_path = resources + "shaders//bounding.vs";
    std::string bbox_fragment_full_path = resources + "shaders//bounding_color.fs";


    std::shared_ptr<Xplor::Shader> simpleShader = std::make_shared<Xplor::Shader>
        (fullVertexPath.c_str(), fullFragmentPath.c_str());
    std::shared_ptr<Xplor::Shader> simpleShaderOneTex = std::make_shared<Xplor::Shader>
        (fullVertexPath.c_str(), fullFragOneTexPath.c_str());
    std::shared_ptr<Xplor::Shader> shaderFlatColor = std::make_shared<Xplor::Shader>
        (fullVertexPath.c_str(), fullFragFlatColorPath.c_str());
    std::shared_ptr<Xplor::Shader>  bbox_shader = std::make_shared<Xplor::Shader>
        (bbox_vertex_full_path.c_str(), bbox_fragment_full_path.c_str());

    simpleShader->Init();
    simpleShaderOneTex->Init();
    shaderFlatColor->Init();


    planeA->AddShader(simpleShaderOneTex);
    auto planeAShader = planeA->GetShader();

    cubeA->AddShader(simpleShader);
    auto cubeAShader = cubeA->GetShader();

    cubeB->AddShader(simpleShaderOneTex);
    auto cubeBShader = cubeB->GetShader();

    // cubeBBOX

    //------ Define shader uniforms
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

    auto geometryPlane = GeometryGenerator::GeneratePlaneData();
    auto planeEBO = GeometryGenerator::GeneratePlaneEBO();

    // Send Geometry information to the game object
    planeA->SetPosition(glm::vec3(-1.f, 0.0f, 1.f));
    planeA->AddGeometry(geometryPlane.data(), geometryPlane.size(), planeEBO.data(), planeEBO.size(), 5);
    planeA->InitGeometry();

    auto geometryCube = GeometryGenerator::GenerateCubeData();
    cubeA->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    cubeA->AddGeometry(geometryCube.data(), geometryCube.size(), 5, 36);
    cubeA->InitGeometry();

    cubeB->SetPosition(glm::vec3(5.0f, 3.0f, -5.0f));
    cubeB->AddGeometry(geometryCube.data(), geometryCube.size(), 5, 36);
    cubeB->InitGeometry();

    // Temporary - Update BBoxes manually to force accurate generation
    planeA->UpdateBoundingBox();
    cubeA->UpdateBoundingBox();
    cubeB->UpdateBoundingBox();


    std::shared_ptr<Xplor::EngineManager> xplorM = Xplor::EngineManager::GetInstance();
    xplorM->AddGameObject(planeA);
    xplorM->AddGameObject(cubeA);
    xplorM->AddGameObject(cubeB);
}

int main(/*int argc, char **argv*/) {
   
    //---- Setup ----
    std::shared_ptr<Xplor::EngineManager> xplorM = Xplor::EngineManager::GetInstance();

    xplorM->CreateWindow(1920, 1080, false);
    std::shared_ptr<WindowManager> windowManager = WindowManager::GetInstance();
    glEnable(GL_DEPTH_TEST);

    windowManager->PrintHardwareInfo();
    PrintMaxVertexAttrib();


    //---- Scene Setup
    constexpr bool EXPORT_SCENE = true;
    constexpr bool IMPORT_SCENE = false;
    if (IMPORT_SCENE)
        xplorM->ImportScene("test.json");
    else
        CreateSceneA();
        

    //---- Camera Setup
    Xplor::CameraVectors camVecs;
    camVecs.camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
    camVecs.camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
    camVecs.camera_up = glm::vec3(0.f , 1.f, 0.f);
    // float cameraSpeed = 3.f;

    xplorM->CreateCamera(camVecs);


    //---- Engine Main Loop ----
    //-----------------------------------------------------
    xplorM->Run();

    if (EXPORT_SCENE)
        xplorM->ExportScene("test.json");

    
    //---- Cleanup ----
    //-----------------------------------------------------
    //cubeA->Delete();

    return 0;
}
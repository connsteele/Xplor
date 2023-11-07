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

struct imgData
{
    int width;
    int height;
    int channels;
    unsigned char* data;
};

int main(int argc, char **argv) {
   
    //---- Setup ----
    WindowManager windowManager;
    windowManager.Init();

    Xplor::PropObject cube;



    //----- Create the engine manager
    //---------------------------------



    //--- Image Loading
    cube.AddTexture("//images//woodBox.jpg", Xplor::ImageFormat::jpg);
    cube.AddTexture("//images//dog.png", Xplor::ImageFormat::png);


    //--- Shader Creation
    //----------------------------------------
    const std::string resources = "..//resources";
    std::string vertexShaderPath = "//shaders//simple.vs";
    std::string fragmentShaderPath = "//shaders//simple.fs";

    //cube.AddShader(vertexShaderPath, fragmentShaderPath);
    std::string fullVertexPath = resources + vertexShaderPath;
    std::string fullFragmentPath = resources + fragmentShaderPath;

    //std::shared_ptr<Xplor::Shader> cubeShader = std::make_shared<Xplor::Shader>(Xplor::Shader(fullVertexPath.c_str(), fullFragmentPath.c_str()));
    Xplor::Shader* simpleShader = new Xplor::Shader(fullVertexPath.c_str(), fullFragmentPath.c_str());
    cube.AddShader(simpleShader);
    auto cubeShader = cube.GetShader();


    //auto cubeShader = cube.GetShader();
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

    cube.AddGeometry(verticesCube, verticesCubeSize, 5);


    //uint32_t VBO, VAO, EBO;
    //glGenVertexArrays(1, &VAO); // Generate one VAO
    //glGenBuffers(1, &VBO); // Generate one buffer object in the OGL Context'
    //glGenBuffers(1, &EBO); // EBO allows us to use indicies for drawing order
    //

    //glBindVertexArray(VAO); // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    //glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the buffer object to a buffer type

    //// Copy data into the buffer object bound to target. The target here
    //// is  GL_ARRAYBUFFER which is bound to the VBO.
    //glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);


    //// Tell OpenGL how to interpret the vertex data per attribute
    //// Here we are accessing the first attribute and checking the 3 vertex points
    //// which are 4 bytes (32bits) each so our stides need to be in steps of 4. We want
    //// to begin at the start of the array
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
    //glEnableVertexAttribArray(0);
    //// define and enable texture coordinates input
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)) );
    //glEnableVertexAttribArray(1);


    // Query hardware information
    windowManager.PrintHardwareInfo();


    
    // Query the max amount of vertex attribs we can use
    int maxAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
    std::cout << "Maxmimum number of vertex attributes supported: " << maxAttributes << std::endl;


    cube.InitGeom();

    // Transformations
    //uint32_t liveTransformLoc = glGetUniformLocation(cubeShader->getID(), "liveTransform");

    //// 3D Coordinates
    //glm::mat4 modelMatrix = glm::mat4(1.0f);
    //modelMatrix = glm::rotate(modelMatrix, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //glm::mat4 viewMatrix = glm::mat4(1.0f);
    //viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f)); // move the camera away from the scene
    //glm::mat4 projectionMatrix;
    //projectionMatrix = glm::perspective(glm::radians(70.0f), 1280.f / 720.f, 0.1f, 100.f); // aspect ratio should be recalced on viewport size change


    //---- Render Loop ----
    //-----------------------------------------------------
    float previousFrameTime = 0.0f;
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 rect_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // The main loop should live in main or the engine manager
    while (!glfwWindowShouldClose(windowManager.m_window)) // Need to setup my own events for this to work better
    {
        //--- Update Delta Time
        float currentFrameTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrameTime - previousFrameTime;
        previousFrameTime = currentFrameTime;

        //--- Input
        //-----------------------------------------------------

        windowManager.ProcessEvents();


        // Rendering commands
        //-----------------------------------------------------

        //--- Background Color
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
            
        //--- OpenGL Rendering
        cube.Render();



        // Swap the front and back buffers
        windowManager.Update();
    }

    
    //---- Cleanup ----
    //-----------------------------------------------------
    cube.Delete();

    return 0;
}
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include "shader.hpp"

// Implement the GLFW callback function prototype
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Set the drawing location to the bottom left of the window and set the rendering area
    // This actually performs the transformation of 2D coordinates to screen locations
    glViewport(0, 0, width, height);


    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

    // Update your projection matrix to match the new aspect ratio
    // For example, if you're using a perspective projection:
    // glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    // glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}


// Handle all incoming keyboard and mouse events
void processInputs(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        // Enable the close window flag
        glfwSetWindowShouldClose(window, true);
    }
}

bool setupImGui(GLFWwindow* window, ImGuiIO& ioOut)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // Enable IO flags
    ioOut = ImGui::GetIO(); (void)ioOut;
    ioOut.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    const char* glsl_version = "#version 330";
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // If everything worked return true
    return true;
}

/// <summary>
/// Create a new ImGui frame using GLFW
/// </summary>
void new_ImGui_Frame()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void printHardwareInfo()
{
    auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    auto openglVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    std::cout << "Vendor: " << vendor << "\nRenderer: " << renderer << std::endl;
    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    std::cout << "OpenGL Version: " << major << "." << minor << std::endl;
}

struct imgData
{
    int width;
    int height;
    int channels;
    unsigned char* data;
};


int main(int argc, char **argv) {
    //---- Setup dependencies ----
    //-----------------------------------------------------
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }



    //---- Set the attributes for the window ----
    //-----------------------------------------------------
    // Set the context to use OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Set OpenGL to Core-profile mode
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Setup a smart pointer for our window and call destroyWindow when it goes out of scope
    int windowWidth = 1280;
    int windowHeight = 720;
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "XplorEngine", NULL, NULL);

    // Cleanup GLFW if the window creation fails
    if (!window)
    {
        std::cout << "Failed to create a GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Register the frame buffer size callback to upate the viewport when the window
    // changes size
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // Give GLAD the address of OpenGL function pointers GLFW
    // will give us the right ones for the OS we are using
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        // GLAD need to have a context to check the OpenGL version against
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::string resources = "..//resources";

    //--- Image Loading
    imgData imageBox;
    stbi_set_flip_vertically_on_load(true); // Align the coordinates
    std::string imagePath = resources + "//images//woodBox.jpg";
    // Fill Variables with image data
    imageBox.data = stbi_load(imagePath.c_str(), &imageBox.width, &imageBox.height, &imageBox.channels, 0);
    if (!imageBox.data)
    {
        std::cout << "Error: Image failed to load" << std::endl;
    }
    //--- Texture generation
    uint32_t texture1;
    // Generate one texture and store it
    glGenTextures(1, &texture1);
    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, texture1);
    // Set the texture filtering and wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // Generate the bound texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageBox.width, imageBox.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageBox.data);
    // Generate mipmaps for the bound texture
    glGenerateMipmap(GL_TEXTURE_2D);
    // Free the image data once the texture has been created
    stbi_image_free(imageBox.data);

    // Get the second texture
    imgData imageDog;
    imagePath = resources + "//images//dog.png";
    imageDog.data = stbi_load(imagePath.c_str(), &imageDog.width, &imageDog.height, &imageDog.channels, 0);
    if (!imageDog.data) 
    {
        std::cout << "Error: Image failed to load" << std::endl;
    }
    uint32_t texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageDog.width, imageDog.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageDog.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imageDog.data);



    // ImGui Setup
    ImGuiIO io;
    if (!setupImGui(window, io))
    {
        // Throw error
    }
   

    //--- Shader Creation
    //----------------------------------------
    std::string vertexShaderPath = resources + "//shaders//simple.vs";
    std::string fragmentShaderPath = resources + "//shaders//simple.fs";

    Xplor::Shader shaderProgram = Xplor::Shader::Shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
    shaderProgram.useProgram();    
    // Inform the shader where the texture samplers are located
    shaderProgram.setInt("customTexture1", 0);
    shaderProgram.setInt("customTexture2", 1);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    shaderProgram.endProgram();


    // Vertex Buffer Setup
    // Rectangle
    float verticesRectangle[] = {
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
    unsigned int indicesRectangle[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // Colored Triangle
    float verticesColorsTriangle[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };
    float triangleTexCoords[] = {
        0.0f, 0.0f, // Bottom Left
        1.0f, 0.0f, // Bottom Right
        0.5f, 1.0f  // Top Middle
        
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



    uint32_t VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO); // Generate one VAO
    glGenBuffers(1, &VBO); // Generate one buffer object in the OGL Context'
    glGenBuffers(1, &EBO); // EBO allows us to use indicies for drawing order
    

    glBindVertexArray(VAO); // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the buffer object to a buffer type
    bool renderRect = true;
    if (renderRect)
    {
        
        // Copy data into the buffer object bound to target. The target here
        // is  GL_ARRAYBUFFER which is bound to the VBO.
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRectangle), verticesRectangle, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesRectangle), indicesRectangle, GL_STATIC_DRAW);


        // Tell OpenGL how to interpret the vertex data per attribute
        // Here we are accessing the first attribute and checking the 3 vertex points
        // which are 4 bytes (32bits) each so our stides need to be in steps of 4. We want
        // to begin at the start of the array
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);
        // define and enable texture coordinates input
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)) );
        glEnableVertexAttribArray(1);
    }
    else
    {
        // Copy colored triangle data into the array buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColorsTriangle), verticesColorsTriangle, GL_STATIC_DRAW);

        // set the stride to 6: We have 3 floats defining vertex pos and 3 defining vertex colors
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);
        // Color attribute, start at an offset of 3 from the beginning of the array
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1); // Enable this vertex attribute
    }



    // Query hardware information
    printHardwareInfo();



    // Query the max amount of vertex attribs we can use
    int maxAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
    std::cout << "Maxmimum number of vertex attributes supported: " << maxAttributes << std::endl;


    // Transformations
    uint32_t liveTransformLoc = glGetUniformLocation(shaderProgram.getID(), "liveTransform");


    // 3D Coordinates
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f)); // move the camera away from the scene
    glm::mat4 projectionMatrix;
    projectionMatrix = glm::perspective(glm::radians(70.0f), 1280.f / 720.f, 0.1f, 100.f); // aspect ratio should be recalced on viewport size change


    //---- Render Loop ----
    //-----------------------------------------------------
    glfwSwapInterval(1); // Enables vsync
    float previousFrameTime = static_cast<float>(glfwGetTime());
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 rect_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window)) // Render till the close flag is true
    {
        //--- Update Delta Time
        float currentFrameTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrameTime - previousFrameTime;
        previousFrameTime = currentFrameTime;

        //--- Input
        //-----------------------------------------------------
        processInputs(window);
        glm::vec3 move(0.0f);
        float moveSpeed = 5.f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            move.y += moveSpeed * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            move.y -= moveSpeed * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            move.x += moveSpeed * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            move.x -= moveSpeed * deltaTime;
        }
        // Rotate the model
        //modelMatrix = glm::translate(modelMatrix, move);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(50.0f) * deltaTime, glm::vec3(0.f, 0.f, 1.0f));

        // Process pending events and update the window state
        glfwPollEvents();

        //--- ImGui
        //-----------------------------------------------------
        new_ImGui_Frame();
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
            ImGui::ColorEdit3("rectangle color", (float*)&rect_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
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

        //--- Background Color
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
            
        //--- OpenGL Rendering
        shaderProgram.useProgram();
        glClear(GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(VAO);
        // glUniform4f(customColorLocation, rect_color.x, rect_color.y, rect_color.z, rect_color.w);
        if (renderRect)
        {
            
            
            // Send coordinate matrices to the shader
            int locModel = glGetUniformLocation(shaderProgram.getID(), "model");
            glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
            int locView = glGetUniformLocation(shaderProgram.getID(), "view");
            glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
            int locProjection = glGetUniformLocation(shaderProgram.getID(), "projection");
            glUniformMatrix4fv(locProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


            glActiveTexture(GL_TEXTURE0); // activate the first texture so it can be bound (this one is bound by default)
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1); // active the second texture so it can also be used
            glBindTexture(GL_TEXTURE_2D, texture2);

            // Draw 10 cubes
            glBindVertexArray(VAO);
            for (unsigned int i = 0; i < 10; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                if (i % 3 == 0)  // every 3rd iteration (including the first) we set the angle using GLFW's time function.
                    angle = glfwGetTime() * 45.0f;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                //shaderProgram.setMat4("model", model);
                glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            // Draw one cube
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw based on indicies
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, 3); // Draw the tris directly
        }
        // Unbinds
        glBindVertexArray(0); // Unbind the VAO
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
        shaderProgram.endProgram();
        

        //--- ImGui Window Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



        // Swap the front and back buffers
        glfwSwapBuffers(window);
    }

    
    //---- Cleanup ----
    //-----------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
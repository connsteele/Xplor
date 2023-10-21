#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include "shader.hpp"

// Implement the GLFW callback function prototype
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Set the drawing location to the bottom left of the window and set the rendering area
    // This actually performs the transformation of 2D coordinates to screen locations
    glViewport(0, 0, width, height);
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


int main(int argc, char **argv) {
    //---- Setup dependencies ----
    //-----------------------------------------------------
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }


    //---- Setup Declerations ------
    //-----------------------------------------------------
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n" 
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 ourColor;\n" // output to frag shader
        "void main()\n"
        "{\n"
        "   ourColor = aColor;\n" 
        "   gl_Position = vec4(aPos, 1.0);\n"
        "}\0";

    const char* fragShaderSource = "#version 330 core\n"
        "in vec3 ourColor;\n"
        "out vec4 FragColor;\n"
        "uniform vec4 customColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(ourColor, 1.0);\n"
        "}\n";


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



    // ImGui Setup
    ImGuiIO io;
    if (!setupImGui(window, io))
    {
        // Throw error
    }
   
    // Change these to be relative to the current dir
    const char* vertexShaderPath = "F://Repos//Xplor//resources//shaders//simple.vs";
    const char* fragmentShaderPath = "F://Repos//Xplor//resources//shaders//simple.fs";

    Xplor::Shader shaderProgram = Xplor::Shader::Shader(vertexShaderPath, fragmentShaderPath);
    shaderProgram.useProgram();

    // Shader Building and Compilation
    //uint32_t vertexShader;
    //vertexShader = glCreateShader(GL_VERTEX_SHADER); // Creates an empty shader object
    //glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Specify shaders and their source
    //glCompileShader(vertexShader);

    //uint32_t fragShader;
    //fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    //glCompileShader(fragShader);

    //// Check for shader compilation errors
    //int32_t shaderCompileStatus;
    //char shaderCompileLog[512];
    //glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderCompileStatus);
    //if (!shaderCompileStatus)
    //{
    //    glGetShaderInfoLog(vertexShader, 512, NULL, shaderCompileLog);
    //    std::cout << "ERROR: SHADER_VERTEX Compilation FAILED\n" << shaderCompileLog << std::endl;
    //}
    //glGetShaderiv(fragShader, GL_COMPILE_STATUS, &shaderCompileStatus);
    //if (!shaderCompileStatus)
    //{
    //    glGetShaderInfoLog(vertexShader, 512, NULL, shaderCompileLog);
    //    std::cout << "ERROR: SHADER_FRAGMENT Compilation FAILED\n" << shaderCompileLog << std::endl;
    //}


    //// Link Shaders to a shader program object
    //uint32_t shaderProgram;
    //shaderProgram = glCreateProgram(); // Create a program object
    //glAttachShader(shaderProgram, vertexShader);
    //glAttachShader(shaderProgram, fragShader);
    //glLinkProgram(shaderProgram);
    //glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &shaderCompileStatus);
    //if (!shaderCompileStatus)
    //{
    //    glGetProgramInfoLog(shaderProgram, 512, NULL, shaderCompileLog);
    //    std::cout << "ERROR: SHADER_PROGRAM Compilation FAILED\n" << shaderCompileLog << std::endl;
    //}
    //glUseProgram(shaderProgram); // If compilation is fine use the program
    //// Once the shaders are linked we can delete them
    //glDeleteShader(vertexShader);
    //glDeleteProgram(fragShader);

    //// Setup Uniforms
    //auto customColorLocation = glGetUniformLocation(shaderProgram, "customColor");
    //if (customColorLocation != -1) {
    //    glUniform4f(customColorLocation, 1.0f, 0.5f, 0.2f, 1.0f); // Set the color value
    //}


    // Vertex Buffer Setup
    // Rectangle
    float verticesRectangle[] = {
     0.5f,  0.5f, 0.0f,  // top right    0
     0.5f, -0.5f, 0.0f,  // bottom right 1
    -0.5f, -0.5f, 0.0f,  // bottom left  2
    -0.5f,  0.5f, 0.0f   // top left     3
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



    uint32_t VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO); // Generate one VAO
    glGenBuffers(1, &VBO); // Generate one buffer object in the OGL Context'
    glGenBuffers(1, &EBO); // EBO allows us to use indicies for drawing order
    

    glBindVertexArray(VAO); // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the buffer object to a buffer type
    bool renderRect = false;
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast <void*>(0));
        glEnableVertexAttribArray(0); // Enable this vertex attribute
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


    //---- Render Loop ----
    //-----------------------------------------------------
    glfwSwapInterval(1); // Enables vsync
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 rect_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window)) // Render till the close flag is true
    {
        //--- Input
        //-----------------------------------------------------
        processInputs(window);
        
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
        glBindVertexArray(VAO);
        // glUniform4f(customColorLocation, rect_color.x, rect_color.y, rect_color.z, rect_color.w);
        if (renderRect)
        {
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw based on indicies
            glBindVertexArray(0); // Unbind the VAO
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, 3); // Draw the tris directly
        }
        
        

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
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

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
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char* fragShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
   

    // Shader Building and Compilation
    uint32_t vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // Creates an empty shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Specify shaders and their source
    glCompileShader(vertexShader);

    uint32_t fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragShader);

    // Check for shader compilation errors
    int32_t shaderCompileStatus;
    char shaderCompileLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderCompileStatus);
    if (!shaderCompileStatus)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, shaderCompileLog);
        std::cout << "ERROR: SHADER_VERTEX Compilation FAILED\n" << shaderCompileLog << std::endl;
    }
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &shaderCompileStatus);
    if (!shaderCompileStatus)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, shaderCompileLog);
        std::cout << "ERROR: SHADER_FRAGMENT Compilation FAILED\n" << shaderCompileLog << std::endl;
    }


    // Link Shaders to a shader program object
    uint32_t shaderProgram;
    shaderProgram = glCreateProgram(); // Create a program object
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &shaderCompileStatus);
    if (!shaderCompileStatus)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, shaderCompileLog);
        std::cout << "ERROR: SHADER_PROGRAM Compilation FAILED\n" << shaderCompileLog << std::endl;
    }
    glUseProgram(shaderProgram); // If compilation is fine use the program
    // Once the shaders are linked we can delete them
    glDeleteShader(vertexShader);
    glDeleteProgram(fragShader);


    // Vertex Buffer Setup
    float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right    0
     0.5f, -0.5f, 0.0f,  // bottom right 1
    -0.5f, -0.5f, 0.0f,  // bottom left  2
    -0.5f,  0.5f, 0.0f   // top left     3
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    uint32_t VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO); // Generate one VAO
    glGenBuffers(1, &VBO); // Generate one buffer object in the OGL Context'
    glGenBuffers(1, &EBO); // EBO allows us to use indicies for drawing order
  
    glBindVertexArray(VAO); // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the buffer object to a buffer type
    // Copy data into the buffer object bound to target. The target here
    // is  GL_ARRAYBUFFER which is bound to the VBO.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // Tell OpenGL how to interpret the vertex data per attribute
    // Here we are accessing the first attribute and checking the 3 vertex points
    // which are 4 bytes (32bits) each so our stides need to be in steps of 4. We want
    // to begin at the start of the array
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast < void*>(0));
    glEnableVertexAttribArray(0); // Enable this vertex attribute


    //---- Render Loop ----
    //-----------------------------------------------------
    glfwSwapInterval(1); // Enables vsync
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
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

        //--- ImGui
        {
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        //--- OpenGL Rendering
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3); // Draw the tris directly
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw based on indicies
        glBindVertexArray(0); // Unbind the VAO




        // Swap the front and back buffers
        glfwSwapBuffers(window);
    }

    
    //---- Cleanup ----
    //-----------------------------------------------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
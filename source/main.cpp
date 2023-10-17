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
    const unsigned int windowWidth = 1280;
    const unsigned int windowHeight = 720;
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

    //---- Render Loop ----
    //-----------------------------------------------------
    while (!glfwWindowShouldClose(window)) // Render till the close flag is true
    {
        //--- Input
        //-----------------------------------------------------
        processInputs(window);


        // Rendering commands
        //-----------------------------------------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the clear color
        glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer with the defined color

        
        // Swap the front and back buffers
        glfwSwapBuffers(window);
        // Process pending events and update the window state
        glfwPollEvents();
    }

    
    //---- Cleanup ----
    //-----------------------------------------------------
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
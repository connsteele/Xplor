#pragma once
#include <iostream>
#include <GL/glew.h>
// Need to add GLEW

// Function to check for OpenGL errors
inline void CheckGLError(const char* file, int line)
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error " << err << " at " << file << ":" << line << std::endl;
    }
}

// Macro to easily insert error checks in the code
#define CHECK_GL_ERROR() CheckGLError(__FILE__, __LINE__)

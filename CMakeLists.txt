cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 17) # Sets C++ Version

# Define the Project name and add source files
project(Xplor-Engine)
set(SOURCES source/main.cpp)
# file(GLOB SOURCES "source/*.cpp") # Just get all .cpp files
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})


#--------------  Dependencies --------------

# The following CMake module will allow us to get deps from online
include(FetchContent)

# Specify where to get GLFW and what version to use
FetchContent_Declare(
    GLFW
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8 # Specify Release Version
)

# Get and build GLFW
FetchContent_MakeAvailable(GLFW)

FetchContent_Declare(
    GLAD
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
)

FetchContent_MakeAvailable(GLAD)

#--------------  Linking --------------

target_link_libraries(Xplor-Engine PRIVATE glfw glad)
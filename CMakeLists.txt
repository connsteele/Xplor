cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 17) # Sets C++ Version

# Define the Project name and add source files
project(Xplor-Engine)
set(SOURCES source/main.cpp)
# file(GLOB SOURCES "source/*.cpp") # Just get all .cpp files
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
# Set start up project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Xplor-Engine)


#--------------  Dependencies --------------

# The following CMake module will allow us to get deps from online
include(FetchContent)

find_package(OpenGL REQUIRED)

# Specify where to get GLFW and what version to use
FetchContent_Declare(
    GLFW
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8 # Specify Release Version
)
# Get and build GLFW
FetchContent_MakeAvailable(GLFW)

# Specify where to get GLAD 
FetchContent_Declare(
    GLAD
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(GLAD)

# Specify where to get Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
)
FetchContent_MakeAvailable(imgui)

# Dear ImGui library configuration
set(IMGUI_BACKENDS "${imgui_SOURCE_DIR}/backends")
add_library(
    imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
)


#--------------  Linking --------------
target_include_directories(
    Xplor-Engine PRIVATE
    ${imgui_SOURCE_DIR}
    ${IMGUI_BACKENDS}
    ${GLAD_BUILD_DIR}/include/glad # Just fixes intellisense issues
)
target_link_libraries(
    Xplor-Engine PRIVATE
    glfw
    glad
    imgui
)

# Link ImGui to OpenGL libs
target_link_libraries(
    imgui PRIVATE
    glad
    glfw
    ${OPENGL_LIBRARY}
)
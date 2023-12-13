cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 17) # Sets C++ Version

# Define the projects name and add source files
project(Xplor-Editor)
project(Xplor-Engine)

set(
    SOURCES_ENGINE
    source/main.cpp
    source/shader.cpp
    source/engine_manager.cpp
    source/matrix_stack.cpp
    source/window_manager.cpp
    source/game_object.cpp
    source/camera.cpp
    source/generator_geometry.cpp
    source/geometry.cpp
    include/xplor_types.hpp
    include/shader.hpp
    include/engine_manager.hpp
    include/matrix_stack.hpp
    include/window_manager.hpp
    include/game_object.hpp
    include/camera.hpp
    include/generator_geometry.hpp
    include/geometry.hpp
    third-party/stb/stb_image.cpp
)

set(
    SOURCES_EDITOR
    source/main.cpp
)

# file(GLOB SOURCES "source/*.cpp") # Just get all .cpp files
# ${CMAKE_PROJECT_NAME} returns the last project set
add_executable(Xplor-Engine ${SOURCES_ENGINE})
add_executable(Xplor-Editor ${SOURCES_EDITOR})
# Set start up project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Xplor-Engine)

if (MSVC)
    target_compile_options(Xplor-Engine PRIVATE /W4)
else ()
    target_compile_options(Xplor-Engine PRIVATE -Wall -Wextra -pedantic)
endif ()

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

# Specify where to get GLM
FetchContent_Declare(
    GLM
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG  0.9.9.8
)
FetchContent_MakeAvailable(GLM)

# Specify where to get Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
)
FetchContent_MakeAvailable(imgui)

# Specify where to get JSON for Modern C++
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Dear ImGui library configuration
set(IMGUI_BACKENDS "${imgui_SOURCE_DIR}/backends")
add_library(
    imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${IMGUI_BACKENDS}/imgui_impl_glfw.cpp
    ${IMGUI_BACKENDS}/imgui_impl_opengl3.cpp
)
target_include_directories(
    imgui PRIVATE
    ${imgui_SOURCE_DIR}
)

#--------------  Linking --------------
include_directories(
    include
    third-party/stb
)

target_include_directories(
    Xplor-Engine PRIVATE
    ${imgui_SOURCE_DIR}
    ${IMGUI_BACKENDS}
    ${GLAD_BUILD_DIR}/include/glad # Just fixes intellisense issues
    ${GLM_SOURCE_DIR}
)

target_link_libraries(
    Xplor-Engine PRIVATE
    glfw
    glad
    glm
    imgui
    nlohmann_json::nlohmann_json
)

# Link ImGui to OpenGL libs
target_link_libraries(
    imgui PRIVATE
    glad
    glfw
    ${OPENGL_LIBRARY}
)
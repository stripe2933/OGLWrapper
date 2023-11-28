cmake_minimum_required(VERSION 3.24)
project(OGLWrapper
    VERSION 0.1.0
    DESCRIPTION "OpenGL Wrapper"
    LANGUAGES CXX
)

add_library(OGLWrapper
    src/Shader.cpp
    src/Texture.cpp
    src/Program.cpp
    src/Helper/Image.cpp
    src/GLFW/GlfwWindow.cpp
)
target_compile_features(OGLWrapper PUBLIC cxx_std_20)

find_package(OpenGL REQUIRED)
find_package(gl3w CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
target_include_directories(OGLWrapper
    PRIVATE
        ${Stb_INCLUDE_DIR}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(OGLWrapper PUBLIC
        OpenGL::GL
        unofficial::gl3w::gl3w
        glfw glm::glm
)
cmake_minimum_required(VERSION 3.5)
project(42run)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
find_package(OpenGL REQUIRED)

#Handle GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(third-party/glfw)
include_directories(third-party/glfw/include)

add_subdirectory(third-party/freetype third-party/freetype/builds)
include_directories(third-party/freetype/include)

include_directories(third-party/glad/include)
include_directories(third-party/glm)
include_directories(third-party/stb)
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(third-party/tinyobjloader)


SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(SOURCE_FILES
        src/main.cpp
        src/env.cpp
        src/Shader.cpp
        src/Camera.cpp
        src/GameObject.cpp
        src/Scene.cpp
        src/Renderer.cpp
        src/Model.cpp
        third-party/glad/src/glad.c
        )

add_executable(42run ${SOURCE_FILES})

target_link_libraries(42run glfw freetype ${GLFW_LIBRARIES})

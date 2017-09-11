cmake_minimum_required(VERSION 3.5)
project(42run)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -g")
find_package(OpenGL REQUIRED)

#Handle GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(third-party/glfw)
include_directories(third-party/glfw/include)
include_directories(third-party/glad/include)
include_directories(third-party/glm)
include_directories(${OPENGL_INCLUDE_DIR})


SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(SOURCE_FILES
        src/main.cpp
        src/env.cpp
        src/Shader.cpp
        src/Camera.cpp
        src/InputHandler.cpp
        third-party/glad/src/glad.c
        )

add_executable(42run ${SOURCE_FILES})

target_link_libraries(42run glfw ${GLFW_LIBRARIES})

#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
// --
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#define MAX_LIGHT 3

struct Light {
  glm::vec4 position;
  glm::vec3 color;
  float radius;
};

struct Vertex {
  glm::vec3 position;
  glm::vec2 uv;
  glm::vec3 normal;
};

static void GL_DUMP_ERROR(std::string message) {
  int glErrorCode = 0;
  if ((glErrorCode = glGetError()) != GL_NO_ERROR)
    std::cout << message << " (error code: " << glErrorCode << ")\n";
}

static void print_matrix(glm::mat4 pMat4) {
  const float *pSource = (const float *)glm::value_ptr(pMat4);
  int i = 0;
  while (i < 16) {
    printf("%f ", pSource[i]);
    if ((i + 1) % 4 == 0) printf("\n");
    i++;
  }
  printf("\n");
}

static void print_vec4(glm::vec4 pvec4) {
  std::cout << pvec4.x << " | " << pvec4.y << " | " << pvec4.z << " | "
            << pvec4.w << std::endl;
}

static void print_vec3(glm::vec3 pvec3) {
  std::cout << pvec3.x << " | " << pvec3.y << " | " << pvec3.z << std::endl;
}

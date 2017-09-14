#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

struct Vertex {
  glm::vec3 position;
  glm::vec2 uv;
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

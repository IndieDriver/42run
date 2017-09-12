#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct Vertex {
  glm::vec3 position;
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

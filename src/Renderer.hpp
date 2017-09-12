#pragma once
#include "env.hpp"
#include "run.hpp"

struct VAO {
  GLuint indice;
  GLsizei size;
};

struct RenderAttrib {
  std::vector<VAO> vaos;
  GLuint shader;
  std::vector<glm::mat4> transforms;
};

class Renderer {
 public:
  Renderer(void);
  Renderer(Renderer const& src);
  virtual ~Renderer(void);
  Renderer& operator=(Renderer const& rhs);
  void draw(const RenderAttrib& renderAttrib);
  glm::mat4 view;
  glm::mat4 proj;
};

#pragma once
#include <algorithm>
#include "env.hpp"
#include "run.hpp"

struct VAO {
  GLuint vao;
  GLuint ebo;
  GLsizei vertices_size;
  GLsizei indices_size;
  VAO(std::vector<Vertex> vertices);
  VAO(std::vector<Vertex> vertices, std::vector<GLuint> indices);
};

struct RenderAttrib {
  VAO* vao;
  GLuint shader;
  std::vector<glm::mat4> transforms;
  glm::vec3 color;  // TODO: replace by texture
  bool operator<(const struct RenderAttrib& rhs) const {
    if (this->shader == rhs.shader) {
      return (this->vao->vao < rhs.vao->vao);
    } else if (this->shader < rhs.shader) {
      return this->shader < rhs.shader;
    } else {
      return this->shader > rhs.shader;
    }
  }
};

class Renderer {
 public:
  Renderer(void);
  Renderer(Renderer const& src);
  virtual ~Renderer(void);
  Renderer& operator=(Renderer const& rhs);
  void addRenderAttrib(const RenderAttrib& renderAttrib);
  void draw();
  void flush();
  glm::mat4 view;
  glm::mat4 proj;
  void printRenderAttribs();

 private:
  std::vector<RenderAttrib> renderAttribs;
};

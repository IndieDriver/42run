#pragma once
#include <algorithm>
#include "env.hpp"
#include "run.hpp"

struct VAO {
  GLuint indice;
  GLsizei size;
  VAO(std::vector<Vertex> vertices) {
    GLuint vbo;
    this->size = vertices.size();
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->size * sizeof(Vertex), vertices.data(),
                 GL_STATIC_DRAW);

    glGenVertexArrays(1, &this->indice);
    glBindVertexArray(this->indice);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, position));

    glEnableVertexAttribArray(0);
  }
};

struct RenderAttrib {
  VAO* vao;
  GLuint shader;
  std::vector<glm::mat4> transforms;
  glm::vec3 color;  // TODO: replace by texture
  bool operator<(const struct RenderAttrib& rhs) const {
    if (this->shader == rhs.shader) {
      return (this->vao->indice < rhs.vao->indice);
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

#include "Renderer.hpp"

Renderer::Renderer(void) {}

Renderer::Renderer(Renderer const &src) { *this = src; }

Renderer::~Renderer(void) {}

Renderer &Renderer::operator=(Renderer const &rhs) {
  if (this != &rhs) {
  }
  return (*this);
}

void Renderer::addRenderAttrib(const RenderAttrib &renderAttrib) {
  renderAttribs.push_back(renderAttrib);
}

void Renderer::draw() {
  std::sort(renderAttribs.begin(), renderAttribs.end());
  int shader_id = -1;
  for (const auto &attrib : this->renderAttribs) {
    if (attrib.shader != shader_id) {
      glUseProgram(attrib.shader);
      shader_id = attrib.shader;
    }
    if (shader_id != -1) {
      glm::mat4 mvp = this->proj * this->view * attrib.transforms[0];
      glUniform3fv(glGetUniformLocation(shader_id, "color"), 1,
                   glm::value_ptr(attrib.color));
      glUniformMatrix4fv(glGetUniformLocation(attrib.shader, "MVP"), 1,
                         GL_FALSE, static_cast<GLfloat *>(glm::value_ptr(mvp)));
      if (attrib.vao->ebo != -1) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attrib.vao->ebo);
        glBindVertexArray(attrib.vao->vao);
        glDrawElements(GL_TRIANGLES, attrib.vao->indices_size, GL_UNSIGNED_INT,
                       static_cast<void *>(0));
      } else {
        glBindVertexArray(attrib.vao->vao);
        glDrawArrays(GL_TRIANGLES, 0, attrib.vao->vertices_size);
      }
    }
  }
}

void Renderer::flush() { renderAttribs.clear(); }

void Renderer::printRenderAttribs() {
  std::cout << "------------" << std::endl;
  for (const auto &attrib : this->renderAttribs) {
    std::cout << attrib.shader << " | " << attrib.vao->vao << std::endl;
  }
  std::cout << "------------" << std::endl;
}

VAO::VAO(std::vector<Vertex> vertices) {
  GLuint vbo;
  this->vao = -1;
  this->ebo = -1;
  this->vertices_size = vertices.size();
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, this->vertices_size * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  glGenVertexArrays(1, &this->vao);
  glBindVertexArray(this->vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid *)offsetof(Vertex, position));
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid *)offsetof(Vertex, uv));

  glEnableVertexAttribArray(0);
  // glEnableVertexAttribArray(1);
}

VAO::VAO(std::vector<Vertex> vertices, std::vector<GLuint> indices) {
  GLuint vbo;
  this->vertices_size = vertices.size();
  this->indices_size = indices.size();
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, this->vertices_size * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  glGenVertexArrays(1, &this->vao);
  glBindVertexArray(this->vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid *)offsetof(Vertex, position));
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid *)offsetof(Vertex, uv));

  glGenBuffers(1, &this->ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  // glEnableVertexAttribArray(1);
}

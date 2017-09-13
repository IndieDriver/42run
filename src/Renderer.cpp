#include "Renderer.hpp"

Renderer::Renderer(void) {}

Renderer::Renderer(Renderer const &src) { *this = src; }

Renderer::~Renderer(void) {}

Renderer &Renderer::operator=(Renderer const &rhs) {
  if (this != &rhs) {
  }
  return (*this);
}

void Renderer::draw(const RenderAttrib &renderAttrib) {
  glUseProgram(renderAttrib.shader);
  for (int i = 0; i < renderAttrib.transforms.size(); i++) {
    glm::mat4 mvp = this->proj * this->view * renderAttrib.transforms[i];
    glUniform3fv(glGetUniformLocation(renderAttrib.shader, "color"), 1,
                 glm::value_ptr(renderAttrib.color));
    glUniformMatrix4fv(glGetUniformLocation(renderAttrib.shader, "MVP"), 1,
                       GL_FALSE, static_cast<GLfloat *>(glm::value_ptr(mvp)));
    glBindVertexArray(renderAttrib.vao->indice);
    glDrawArrays(GL_TRIANGLES, 0, renderAttrib.vao->size);
  }
}

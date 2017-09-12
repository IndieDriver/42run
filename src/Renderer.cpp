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
  // print_matrix(this->proj);
  // print_matrix(this->view);
  glUseProgram(renderAttrib.shader);
  glm::mat4 model;
  for (int i = 0; i < renderAttrib.vaos.size(); i++) {
    if (i < renderAttrib.transforms.size()) {
      model = renderAttrib.transforms[i];
    }
    VAO vao = renderAttrib.vaos[i];
    glm::mat4 mvp = this->proj * this->view * model;
    // print_matrix(mvp);
    glUniformMatrix4fv(glGetUniformLocation(renderAttrib.shader, "MVP"), 1,
                       GL_FALSE, static_cast<GLfloat *>(&mvp[0][0]));
    glBindVertexArray(vao.indice);
    glDrawArrays(GL_TRIANGLES, 0, vao.size);
  }
}

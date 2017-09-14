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
      glBindVertexArray(attrib.vao->indice);
      glDrawArrays(GL_TRIANGLES, 0, attrib.vao->size);
    }
  }
}

void Renderer::flush() { renderAttribs.clear(); }

void Renderer::printRenderAttribs() {
  std::cout << "------------" << std::endl;
  for (const auto &attrib : this->renderAttribs) {
    std::cout << attrib.shader << " | " << attrib.vao->indice << std::endl;
  }
  std::cout << "------------" << std::endl;
}

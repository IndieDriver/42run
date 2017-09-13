#include "Model.hpp"

Model::Model(void){};

Model::Model(GLuint shader, VAO& vao) {
  this->_renderAttrib.shader = shader;
  this->_renderAttrib.vao = &vao;
  this->_renderAttrib.transforms.push_back(glm::mat4(1.0f));
}

Model::Model(Model const& src) { *this = src; }

Model::~Model(void) {}

Model& Model::operator=(Model const& rhs) {
  if (this != &rhs) {
    this->_renderAttrib = rhs._renderAttrib;
  }
  return (*this);
}

// void Model::update() {}

const RenderAttrib Model::getRenderAttrib() const {
  return (this->_renderAttrib);
}

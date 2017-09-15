#include "Model.hpp"

Model::Model(void){};

Model::Model(GLuint shader, VAO* vao, Texture* texture, glm::vec3 pos,
             glm::vec3 rot, glm::vec3 scale)
    : _position(pos), _rotation(rot), _scale(scale) {
  this->_renderAttrib.shader = shader;
  this->_renderAttrib.vao = vao;
  this->_renderAttrib.transforms.push_back(glm::mat4(1.0f));
  this->_renderAttrib.texture = texture;
  updateTransform();
}

Model::Model(Model const& src) { *this = src; }

Model::~Model(void) {
  // delete this->_renderAttrib.vao;
}

Model& Model::operator=(Model const& rhs) {
  if (this != &rhs) {
    this->_renderAttrib = rhs._renderAttrib;
    this->_position = rhs._position;
    this->_rotation = rhs._rotation;
    this->_scale = rhs._scale;
  }
  return (*this);
}

void Model::setTransform(glm::vec3 position, glm::vec3 rotation,
                         glm::vec3 scale) {
  this->_position = position;
  this->_rotation = rotation;
  this->_scale = scale;
  updateTransform();
}

void Model::setTransform(glm::mat4 transform) {
  this->_renderAttrib.transforms[0] = transform;
}

glm::mat4 Model::getTransform() {
  updateTransform();
  return (this->_renderAttrib.transforms[0]);
}

void Model::updateTransform() {
  glm::mat4 mat_translation = glm::translate(_position);
  glm::mat4 mat_rotation =
      glm::eulerAngleYXZ(_rotation.y, _rotation.x, _rotation.z);
  glm::mat4 mat_scale = glm::scale(_scale);
  this->_renderAttrib.transforms[0] =
      mat_translation * mat_rotation * mat_scale;
}

const RenderAttrib Model::getRenderAttrib() const {
  return (this->_renderAttrib);
}

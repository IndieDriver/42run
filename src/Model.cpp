#include "Model.hpp"

Model::Model(void) {}

Model::Model(GLuint shader, std::vector<Vertex> vertices) {
  GLuint vbo;
  VAO vao;

  vao.size = vertices.size();
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vao.size * sizeof(Vertex), vertices.data(),
               GL_STATIC_DRAW);

  glGenVertexArrays(1, &vao.indice);
  glBindVertexArray(vao.indice);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid*)offsetof(Vertex, position));

  glEnableVertexAttribArray(0);

  this->_renderAttrib.shader = shader;
  this->_renderAttrib.vaos.push_back(vao);
  this->_renderAttrib.transforms.push_back(glm::mat4(1.0f));
}

Model::Model(Model const& src) { *this = src; }

Model::~Model(void) {}

Model& Model::operator=(Model const& rhs) {
  if (this != &rhs) {
  }
  return (*this);
}

const RenderAttrib Model::getRenderAttrib() const {
  return (this->_renderAttrib);
}

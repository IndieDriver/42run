#pragma once
#include <array>
#include "Renderer.hpp"
#include "env.hpp"
#include "run.hpp"

class Model {
 public:
  Model(GLuint shader, VAO* vao, Texture* texture,
        glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
  Model(Model const& src);
  virtual ~Model(void);
  Model& operator=(Model const& rhs);
  const RenderAttrib getRenderAttrib() const;
  void add(glm::mat4 transform);
  void setTransform(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
  void setTransform(glm::mat4 transform);
  glm::mat4 getTransform();
  void updateTransform();

  void setTexture(Texture* texture);

 private:
  Model(void);
  RenderAttrib _renderAttrib;
  glm::vec3 _position;
  glm::vec3 _rotation;
  glm::vec3 _scale;
};

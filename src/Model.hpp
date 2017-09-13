#pragma once
#include "Renderer.hpp"
#include "env.hpp"
#include "run.hpp"

class Model {
 public:
  Model(GLuint shader, VAO& vao);
  Model(Model const& src);
  virtual ~Model(void);
  Model& operator=(Model const& rhs);
  const RenderAttrib getRenderAttrib() const;
  void add(glm::mat4 transform);

 private:
  Model(void);
  RenderAttrib _renderAttrib;
};

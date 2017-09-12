#pragma once
#include "Renderer.hpp"
#include "env.hpp"
#include "run.hpp"

class Model {
 public:
  Model(GLuint shader, std::vector<Vertex> vertices);
  Model(Model const& src);
  virtual ~Model(void);
  Model& operator=(Model const& rhs);
  const RenderAttrib getRenderAttrib() const;

 private:
  Model(void);
  RenderAttrib _renderAttrib;
};

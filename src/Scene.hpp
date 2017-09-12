#pragma once
#include "Model.hpp"
#include "run.hpp"

class Scene {
 public:
  Scene(void);
  Scene(Scene const& src);
  virtual ~Scene(void);
  Scene& operator=(Scene const& rhs);
  std::vector<Model> model;
};

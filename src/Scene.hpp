#pragma once
#include "Model.hpp"
#include "Renderer.hpp"
#include "run.hpp"

class Scene {
 public:
  Scene(Renderer* renderer);
  Scene(Scene const& src);
  virtual ~Scene(void);
  Scene& operator=(Scene const& rhs);
  std::vector<Model> models;
  void draw();

 private:
  Scene(void);
  Renderer* _renderer;
};

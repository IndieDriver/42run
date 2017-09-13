#pragma once
#include "Model.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "run.hpp"

class Scene {
 public:
  Scene(Shader shader, Renderer* renderer);
  Scene(Scene const& src);
  virtual ~Scene(void);
  Scene& operator=(Scene const& rhs);
  std::vector<Model> models;
  Model* cubeModel;
  void draw();
  void loadFloor(glm::vec3 startPosition);

 private:
  Scene(void);
  Renderer* _renderer;
};

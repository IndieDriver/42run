#pragma once
#include "Model.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "run.hpp"

class Floor {
 public:
  std::array<int, 81> setup;
  glm::mat4 transform;
  std::vector<Model> models;
  Floor(std::array<int, 81> init) { setup = init; }
};

class Scene {
 public:
  Scene(Shader shader, Renderer* renderer);
  Scene(Scene const& src);
  virtual ~Scene(void);
  Scene& operator=(Scene const& rhs);
  std::vector<Model> models;
  std::vector<Floor*> floors;
  Model* cubeModel;
  void init();
  void draw();
  void populateFloor(Floor* floor_ptr, glm::vec3 startPosition);

 private:
  Scene(void);
  Renderer* _renderer;
};

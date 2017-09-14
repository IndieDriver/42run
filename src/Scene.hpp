#pragma once
#include <deque>
#include "Model.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "run.hpp"

class Floor {
 public:
  std::array<int, 81> setup;
  glm::mat4 transform;
  std::vector<Model> models;
  Floor(std::array<int, 81> init);
  void move(glm::vec3 pos);
  void updateTransform();
  void setTransform(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
  glm::vec3 getPosition();
  glm::vec3 getRotation();

 private:
  glm::vec3 _position;
  glm::vec3 _rotation;
  glm::vec3 _scale;
};

struct Player {
  glm::vec3 pos = glm::vec3(0.0f);
};

class Scene {
 public:
  Scene(Shader shader, Renderer* renderer);
  Scene(Scene const& src);
  virtual ~Scene(void);
  Scene& operator=(Scene const& rhs);
  std::vector<Model> models;
  std::deque<Floor*> floors;
  Model* cubeModel;
  void init();
  void update(std::array<bool, 1024> keys);
  void draw();
  void populateFloor(Floor* floor_ptr);

 private:
  Scene(void);
  Renderer* _renderer;
  Player player;
};

#pragma once
#include <deque>
#include <limits>
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
  float speed = {5.0f};
  glm::vec3 position = {0.0f, 0.0f, 0.0f};
  glm::vec3 velocity = {0.0f, 0.0f, 0.0f};

  void update(std::array<bool, 1024> keys, float deltaTime);
  glm::vec3 offsetSinceLastFrame = {0.0f, 0.0f, 0.0f};
};

class Scene {
 public:
  Scene(Shader shader, Renderer* renderer);
  Scene(Scene const& src);
  virtual ~Scene(void);
  Scene& operator=(Scene const& rhs);
  std::vector<Model> models;
  std::deque<Floor*> floors;
  std::vector<Texture*> floor_textures;
  std::vector<Texture*> wall_textures;
  std::vector<Texture*> root_textures;
  Model* cubeModel;
  void init();
  void update(std::array<bool, 1024> keys, float deltaTime);
  void draw();
  void populateFloor(Floor* floor_ptr);

 private:
  Scene(void);
  void drawUI();
  Renderer* _renderer;
  Player _player;
  float _meter_counter;
};

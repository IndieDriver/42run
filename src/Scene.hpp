#pragma once
#include <deque>
#include <limits>
#include "Camera.hpp"
#include "GameObject.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "run.hpp"

struct Floor {
  std::array<int, 81> setup;
  std::vector<GameObject*> entities;
};

class Scene {
 public:
  Scene(Shader shader, Camera* camera, Renderer* renderer);
  Scene(Scene const& src);
  virtual ~Scene(void);
  Scene& operator=(Scene const& rhs);
  std::deque<GameObject*> floors;
  std::vector<Texture*> floor_textures;
  std::vector<Texture*> wall_textures;
  std::vector<Texture*> roof_textures;
  World world;
  VAO* vao_cube;
  void update(InputHandler& inputHandler, float deltaTime);
  void draw();
  void drawPauseUI();
  void populateFloor(GameObject* floor_ptr, const Floor& setup);

  GLuint shader_id;
  std::vector<Floor> floor_pool;
  std::vector<GameObject*> obstacle_pool;

 private:
  Scene(void);
  void drawUI();
  Camera* _camera;
  Renderer* _renderer;
  GameObject* _player;
  float _meter_counter;
  bool _paused;
};

#pragma once
#include <deque>
#include <limits>
#include "Camera.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "run.hpp"

class Scene {
 public:
  Scene(Shader shader, Camera* camera, Renderer* renderer, VAO* cube);
  Scene(Scene const& src);
  virtual ~Scene(void);
  Scene& operator=(Scene const& rhs);
  std::deque<GameObject*> floors;
  std::vector<Texture*> floor_textures;
  std::vector<Texture*> wall_textures;
  std::vector<Texture*> roof_textures;
  World world;
  VAO* vao_cube;
  GameObject* cubeModel;
  void init();
  void update(InputHandler& inputHandler, float deltaTime);
  void draw();
  void populateFloor(GameObject* floor_ptr, std::array<int, 81> setup);

  GLuint shader;

 private:
  Scene(void);
  void drawUI();
  Camera* _camera;
  Renderer* _renderer;
  GameObject* _player;
  float _meter_counter;
};

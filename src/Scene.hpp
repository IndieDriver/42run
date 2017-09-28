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
  void update(InputHandler& inputHandler, float deltaTime);
  void draw();

  World world;
  VAO* vao_cube;
  std::deque<GameObject*> floors;
  std::vector<Texture*> floor_textures_pool;
  std::vector<Texture*> wall_textures_pool;
  std::vector<Texture*> roof_textures_pool;

  GLuint shader_id;
  std::vector<Floor> floor_pool;
  std::vector<GameObject*> obstacle_pool;

 private:
  Scene(void);
  void drawUI();
  void drawPauseUI();

  void popOldFloor();
  void pushNewFloor();
  void populateFloor(GameObject* floor_ptr, const Floor& setup);

  void pushObstacleModel(std::string model_filename,
                         std::string texture_filename);
  void createPlayer();

  Texture* addTexture(std::string filename);
  VAO* addVAO(std::string filename);

  void addObstacle(GameObject* parent);
  Camera* _camera;
  Renderer* _renderer;
  GameObject* _player;
  float _meter_counter;
  bool _paused;
  int _difficulty;

  std::vector<VAO*> _scene_vaos;
  std::vector<Texture*> _scene_textures;
};

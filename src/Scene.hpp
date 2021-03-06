#include <deque>
#include <limits>
#include "Camera.hpp"
#include "GameObject.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "run.hpp"

class Scene {
 public:
  Scene(Shader shader, Camera* camera, Renderer* renderer);
  Scene(Scene const& src);
  virtual ~Scene(void);
  Scene& operator=(Scene const& rhs);
  void update(InputHandler& inputHandler, float deltaTime);
  void draw();

  World world;
  GameObject* back_floor;

  GLuint shader_id;
  std::vector<GameObject*> floor_pool;
  std::vector<GameObject*> obstacle_pool;

 private:
  Scene(void);
  void drawUI();
  void drawPauseUI();
  void drawGameOverUI();

  void cleanup();
  void pushNewFloor();
  void populateFloor(GameObject* floor_ptr);

  void pushObstacleModel(std::string model_filename,
                         std::string texture_filename, glm::vec3 scale);
  void createPlayer();

  Texture* addTexture(std::string filename);
  VAO* addVAO(std::string filename);

  GameObject* getObstacle(glm::vec3 floorPos);
  Camera* _camera;
  Renderer* _renderer;
  GameObject* _player;
  float _meter_counter;
  bool _paused;
  bool _ended;
  int _difficulty;

  std::vector<VAO*> _scene_vaos;
  std::vector<Texture*> _scene_textures;
};

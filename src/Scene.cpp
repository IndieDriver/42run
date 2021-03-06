#include "Scene.hpp"

Scene::Scene(void) {}

Scene::Scene(Shader shader, Camera* camera, Renderer* renderer)
    : back_floor(nullptr),
      shader_id(shader.id),
      _camera(camera),
      _renderer(renderer),
      _player(nullptr),
      _meter_counter(-1),
      _paused(false),
      _ended(false),
      _difficulty(0) {
  VAO* floor_vao = addVAO("models/floor2.obj");
  this->floor_pool.push_back(new GameObject(shader_id, floor_vao,
                                            addTexture("textures/floor4.png"),
                                            nullptr, nullptr, nullptr));
  this->floor_pool.push_back(new GameObject(shader_id, floor_vao,
                                            addTexture("textures/floor5.png"),
                                            nullptr, nullptr, nullptr));

  pushObstacleModel("models/chair.obj", "textures/chair.png",
                    glm::vec3(0.6f, 0.6f, 0.6f));
  pushObstacleModel("models/table3.obj", "textures/table2.png",
                    glm::vec3(0.6f, 0.6f, 0.6f));
  pushObstacleModel("models/trash.obj", "textures/trash.png",
                    glm::vec3(0.6f, 0.6f, 0.6f));

  createPlayer();

  pushNewFloor();
  pushNewFloor();
  pushNewFloor();
  pushNewFloor();
}

Scene::Scene(Scene const& src) { *this = src; }

Scene::~Scene(void) {
  for (GameObject* entity : world.entities) {
    delete entity;
  }
  for (GameObject* obs : obstacle_pool) {
    delete obs;
  }
  for (GameObject* obs : floor_pool) {
    delete obs;
  }
  for (VAO* vao : _scene_vaos) {
    delete vao;
  }
  for (Texture* tex : _scene_textures) {
    delete tex;
  }
}

Scene& Scene::operator=(Scene const& rhs) {
  if (this != &rhs) {
    this->back_floor = rhs.back_floor;
    this->world = rhs.world;
    this->shader_id = rhs.shader_id;
    this->floor_pool = rhs.floor_pool;
    this->obstacle_pool = rhs.obstacle_pool;
    this->_camera = rhs._camera;
    this->_renderer = rhs._renderer;
    this->_player = rhs._player;
    this->_meter_counter = rhs._meter_counter;
    this->_paused = rhs._paused;
    this->_ended = rhs._ended;
    this->_difficulty = rhs._difficulty;
    this->_scene_vaos = rhs._scene_vaos;
    this->_scene_textures = rhs._scene_textures;
  }
  return (*this);
}

void Scene::update(InputHandler& inputHandler, float deltaTime) {
  if (this->_ended) return;
  if (inputHandler.keys[GLFW_KEY_P]) {
    this->_paused = !this->_paused;
    inputHandler.keys[GLFW_KEY_P] = false;
  }
  if (this->_paused) return;
  if (this->_difficulty >= 4) {
    this->_difficulty = roundf(0.2f * floor(_meter_counter / 20) + 4.0f);
    this->_difficulty = glm::clamp(this->_difficulty, 0, 10);
  }
  if (this->back_floor != nullptr &&
      this->back_floor->transform.position.z -
              this->_player->transform.position.z <
          125.0f) {
    pushNewFloor();
  }
  cleanup();
  world.update(inputHandler, deltaTime);

  if (this->_player != nullptr) {
    if (_player->physicsComponent->has_collide) {
      this->_ended = true;
    }
    /* _camera->pos = _player->transform.position + glm::vec3(0.0f, 1.5f,
     * -1.5f); */
    _camera->pos = glm::vec3(0.0f, 0.0f, _player->transform.position.z) +
                   glm::vec3(0.0f, 1.5f, -1.5f);
    _camera->update();
    _renderer->view = this->_camera->view;
    _renderer->proj = this->_camera->proj;
    this->_meter_counter += this->_player->positionRelative.z;
  }
}

void Scene::draw() {
  for (const auto entity : world.entities) {
    this->_renderer->addRenderAttrib(entity->getRenderAttrib());
  }
  this->_renderer->draw();
  this->_renderer->flush();
  if (this->_paused) {
    drawPauseUI();
  }
  if (this->_ended) {
    drawGameOverUI();
  }
  drawUI();
}

void Scene::drawUI() {
  this->_renderer->renderText(
      this->_renderer->getScreenWidth() - 150.0f,
      this->_renderer->getScreenHeight() - 50.0f, 1.0f,
      std::to_string(static_cast<int>(this->_meter_counter)),
      glm::vec3(0.0f, 0.0f, 0.0f));
}

void Scene::drawPauseUI() {
  this->_renderer->renderText(
      (this->_renderer->getScreenWidth() / 2.0f) - 75.0f,
      this->_renderer->getScreenHeight() / 2.0f, 1.0f, "Pause",
      glm::vec3(0.0f, 0.0f, 0.0f));
}

void Scene::drawGameOverUI() {
  this->_renderer->renderText(
      (this->_renderer->getScreenWidth() / 2.0f) - 150.0f,
      this->_renderer->getScreenHeight() / 2.0f, 1.0f, "Game Over",
      glm::vec3(0.0f, 0.0f, 0.0f));
}

void Scene::cleanup() {
  glm::vec3 playerPos = this->_player->transform.position;
  // Remove everything behind the player
  world.entities.erase(
      std::remove_if(world.entities.begin(), world.entities.end(),
                     [playerPos](GameObject* go) {
                       if (go->transform.position.z - playerPos.z < -10.0f) {
                         delete go;
                         return (true);
                       }
                       return (false);
                     }),
      world.entities.end());
  this->_renderer->lights.erase(
      std::remove_if(this->_renderer->lights.begin(),
                     this->_renderer->lights.end(),
                     [playerPos](Light light) {
                       if (light.position.z - playerPos.z < -10.0f) {
                         return (true);
                       }
                       return (false);
                     }),
      this->_renderer->lights.end());
}

void Scene::pushNewFloor() {
  Light light;
  glm::vec3 floorPos;
  if (this->back_floor == nullptr) {
    floorPos = glm::vec3(0.0f, -1.0f, 0.0f);
  } else {
    floorPos = this->back_floor->transform.position;
    floorPos.z += 20.0f;
  }
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(0, floor_pool.size() - 1);
  int rand_nb = dist(mt);
  GameObject* newFloor = new GameObject(*this->floor_pool[dist(mt)]);

  newFloor->transform.position = floorPos;
  populateFloor(newFloor);

  light.position = glm::vec4(floorPos, 0.0f);
  light.position.z += 10.0f;
  light.position.y += 5.0f;
  light.color = glm::vec3(1.0f, 1.0f, 1.0f);
  light.radius = 20.0f;
  this->_renderer->lights.push_back(light);

  this->world.entities.push_back(newFloor);
  this->back_floor = newFloor;
  if (this->_difficulty < 4) this->_difficulty++;
}

void Scene::populateFloor(GameObject* floor_ptr) {
  std::vector<GameObject*> obstacles;
  std::random_device rd;
  std::mt19937 mt(rd());
  for (int i = 0; i < this->_difficulty; i++) {
    std::uniform_int_distribution<int> dist(0, 2);
    int rand_nb = dist(mt);
    if (rand_nb == 0) {
      GameObject* obs = getObstacle(floor_ptr->transform.position);
      if (obs != nullptr) {
        for (GameObject* obstacle : obstacles) {
          if (glm::distance(obs->transform.position,
                            obstacle->transform.position) < 1.5f) {
            delete obs;
            obs = nullptr;
            break;
          }
        }
        if (obs != nullptr) {
          obstacles.push_back(obs);
        }
      }
    }
  }
  for (GameObject* obstacle : obstacles) {
    world.entities.push_back(obstacle);
  }
}

Texture* Scene::addTexture(std::string filename) {
  Texture* texture = new Texture(filename);
  this->_scene_textures.push_back(texture);
  return (texture);
}

VAO* Scene::addVAO(std::string filename) {
  Model model(filename);
  VAO* vao = new VAO(model.vertices);
  this->_scene_vaos.push_back(vao);
  return (vao);
}

void Scene::pushObstacleModel(std::string model_filename,
                              std::string texture_filename, glm::vec3 scale) {
  VAO* vao = addVAO(model_filename);
  Texture* texture = addTexture(texture_filename);
  GameObject* gameObject =
      new GameObject(shader_id, vao, texture, nullptr, nullptr, nullptr);
  gameObject->transform.scale = scale;

  this->obstacle_pool.push_back(gameObject);
}

GameObject* Scene::getObstacle(glm::vec3 floor_pos) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist_pos(0, 10);
  glm::vec3 obstacle_pos =
      glm::vec3(0.0f, 0.0f, static_cast<float>(dist_pos(mt) * 2.0f));
  if (obstacle_pool.size() > 0) {
    std::uniform_int_distribution<int> dist_rail(0, 2);
    int rand_rail = dist_rail(mt);
    if (rand_rail == 0) {  // Rail left
      obstacle_pos.x += 1.25f;
    } else if (rand_rail == 2) {  // Rail right
      obstacle_pos.x -= 1.25f;
    }
    std::uniform_int_distribution<int> dist_obs(0, obstacle_pool.size() - 1);
    GameObject* obstacle = obstacle_pool[dist_obs(mt)];
    GameObject* newObstacle = new GameObject(*obstacle);
    newObstacle->transform.position = floor_pos + obstacle_pos;
    std::uniform_int_distribution<float> dist_rot(0, 360);
    newObstacle->transform.rotation.y = glm::radians(dist_rot(mt));
    newObstacle->is_collider = true;
    return (newObstacle);
  }
  return (nullptr);
}

void Scene::createPlayer() {
  VAO* marvin_vao = addVAO("models/marvin.obj");
  Texture* texture = addTexture("textures/marvin_tex.png");
  GameObject* player =
      new GameObject(shader_id, marvin_vao, texture, new InputComponent(),
                     new PhysicsComponent(), nullptr);
  player->transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
  player->is_collider = true;
  this->_player = player;
  world.entities.push_back(player);
}

#include "Scene.hpp"

Scene::Scene(void) {}

Scene::Scene(Shader shader, Camera* camera, Renderer* renderer)
    : shader_id(shader.id),
      _camera(camera),
      _renderer(renderer),
      _player(nullptr),
      _meter_counter(-1),
      _paused(false),
      _difficulty(0) {
  VAO* marvin_vao = addVAO("models/floor1.obj");
  Texture* texture = addTexture("textures/floor_tex.png");
  this->floor_pool.push_back(new GameObject(shader_id, marvin_vao, texture,
                                            nullptr, nullptr, nullptr));

  this->floor_textures_pool.push_back(addTexture("textures/floor_black.png"));
  this->wall_textures_pool.push_back(addTexture("textures/white_wall.png"));
  this->wall_textures_pool.push_back(addTexture("textures/wood_tex.png"));

  this->vao_cube = addVAO("models/cube.obj");

  pushObstacleModel("models/table.obj", "models/table.png");
  pushObstacleModel("models/marvin.obj", "models/table.png");

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
  for (VAO* vao : _scene_vaos) {
    delete vao;
  }
  for (Texture* tex : _scene_textures) {
    delete tex;
  }
}

Scene& Scene::operator=(Scene const& rhs) {
  if (this != &rhs) {
    this->floors = rhs.floors;
    this->floor_textures_pool = rhs.floor_textures_pool;
    this->wall_textures_pool = rhs.wall_textures_pool;
    this->roof_textures_pool = rhs.roof_textures_pool;
    this->world = rhs.world;
    this->vao_cube = rhs.vao_cube;
    this->shader_id = rhs.shader_id;
    this->floor_pool = rhs.floor_pool;
    this->obstacle_pool = rhs.obstacle_pool;
    this->_camera = rhs._camera;
    this->_renderer = rhs._renderer;
    this->_player = rhs._player;
    this->_meter_counter = rhs._meter_counter;
    this->_paused = rhs._paused;
    this->_difficulty = rhs._difficulty;
    this->_scene_vaos = rhs._scene_vaos;
    this->_scene_textures = rhs._scene_textures;
  }
  return (*this);
}

void Scene::update(InputHandler& inputHandler, float deltaTime) {
  if (inputHandler.keys[GLFW_KEY_P]) {
    this->_paused = !this->_paused;
    inputHandler.keys[GLFW_KEY_P] = false;
  }
  if (this->_paused) return;
  this->_difficulty = floor(fmod(_meter_counter, 20));
  this->_difficulty = glm::clamp(this->_difficulty, 0, 20);
  if (this->floors.size() > 0 && this->floors.back()->transform.position.z -
                                         this->_player->transform.position.z <
                                     70.0f) {
    pushNewFloor();
  }
  cleanup();
  world.update(inputHandler, deltaTime);

  if (this->_player != nullptr) {
    if (_player->physicsComponent->has_collide) {
      this->_paused = true;
    }
    _camera->pos = _player->transform.position + glm::vec3(0.0f, 0.5f, -2.0f);
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
  drawUI();
}

void Scene::drawUI() {
  this->_renderer->renderText(
      this->_renderer->getScreenWidth() - 100.0f,
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

void Scene::cleanup() {
  glm::vec3 playerPos = this->_player->transform.position;
  world.entities.erase(
      std::remove_if(world.entities.begin(), world.entities.end(),
                     [playerPos](GameObject* go) {
                       if (go->transform.position.z - playerPos.z < -5.0f) {
                         delete go;
                         return (true);
                       }
                       return (false);
                     }),
      world.entities.end());
}

void Scene::pushNewFloor() {
  glm::vec3 floorPos;
  if (this->floors.size() == 0) {
    floorPos = glm::vec3(0.0f, -1.0f, 0.0f);
  } else {
    floorPos = this->floors.back()->transform.position;
    floorPos.z += 19.0f;
  }
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(0, floor_pool.size() - 1);
  int rand_nb = dist(mt);
  GameObject* newFloor = new GameObject(*this->floor_pool[dist(mt)]);

  newFloor->transform.position = floorPos;

  this->world.entities.push_back(newFloor);
  this->floors.push_back(newFloor);
  std::cout << "world entities: " << this->world.entities.size() << std::endl;
}

void Scene::populateFloor(GameObject* floor_ptr) {
  std::random_device rd;
  std::mt19937 mt(rd());
  for (int i = 0; i < this->_difficulty; i++) {
    std::uniform_int_distribution<int> dist(0, 2);
    int rand_nb = dist(mt);
    if (rand_nb == 0) {
      addObstacle(floor_ptr);
    }
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
                              std::string texture_filename) {
  VAO* vao = addVAO(model_filename);
  Texture* texture = addTexture(texture_filename);
  GameObject* gameObject =
      new GameObject(shader_id, vao, texture, nullptr, nullptr, nullptr);

  this->obstacle_pool.push_back(gameObject);
}

void Scene::addObstacle(GameObject* parent) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist_pos(0, 8);
  glm::vec3 obstacle_pos =
      glm::vec3(4.0f, -0.5f, static_cast<float>(dist_pos(mt)));
  if (obstacle_pool.size() > 0) {
    std::uniform_int_distribution<int> dist_rail(0, 2);
    int rand_rail = dist_rail(mt);
    if (rand_rail == 0) {  // Rail left
      obstacle_pos.x += 0.25f;
    } else if (rand_rail == 2) {  // Rail right
      obstacle_pos.x -= 0.25f;
    }
    std::cout << "new obstacles at: ";
    print_vec3(obstacle_pos);
    std::uniform_int_distribution<int> dist_obs(0, obstacle_pool.size() - 1);
    GameObject* obstacle = obstacle_pool[dist_obs(mt)];
    GameObject* newObstacle = new GameObject(*obstacle);
    newObstacle->parent = parent;
    newObstacle->transform.position = obstacle_pos;
    newObstacle->transform.scale = glm::vec3(0.08f, 0.08f, 0.08f);
    newObstacle->updateAABB();
    newObstacle->is_collider = true;
    /* world.entities.push_back(newObstacle); */
  }
}

void Scene::createPlayer() {
  VAO* marvin_vao = addVAO("models/marvin.obj");
  std::cout << "vao size: " << marvin_vao->vertices.size() << std::endl;
  std::cout << "vao indices: " << marvin_vao->indices_size << std::endl;
  Texture* texture = addTexture("textures/wood_tex.png");
  GameObject* player =
      new GameObject(shader_id, marvin_vao, texture, new InputComponent(),
                     new PhysicsComponent(), nullptr);
  // player->aabb_min = glm::vec3(-0.2f, 0.1f, -0.2f);
  // player->aabb_max = glm::vec3(0.2f, 0.5f, 0.2f);
  print_vec3(player->aabb_min);
  print_vec3(player->aabb_max);
  // player->transform.scale = glm::vec3(0.01f, 0.01f, 0.01f);
  // player->transform.scale = glm::vec3(4.0f, 4.0f, 4.0f);
  // player->transform.rotation = glm::vec3(90.0f, 0.0f, 0.0f);
  player->transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
  player->updateAABB();
  print_vec3(player->aabb_min);
  print_vec3(player->aabb_max);
  print_vec3(player->transform.position);
  player->is_collider = true;
  this->_player = player;
  std::cout << "scale: ";
  print_vec3(this->_player->transform.scale);
  world.entities.push_back(player);
}

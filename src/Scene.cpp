#include "Scene.hpp"

Floor floor_setup1 = {{
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1   //
                      },
                      {{4.0f, 0.0f, 3.0f}, {4.0f, 0.0f, 7.0f}}};

Floor floor_setup2 = {{
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 0, 0, 0, 1, 1, 1,  //
                          1, 1, 1, 0, 0, 0, 1, 1, 1,  //
                          1, 1, 1, 0, 0, 0, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1   //
                      },
                      {{4.0f, 0.0f, 3.0f}, {4.0f, 0.0f, 7.0f}}};

Floor floor_setup3 = {{
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 2, 0, 2, 1, 1, 1,  //
                          1, 1, 1, 2, 0, 2, 1, 1, 1,  //
                          1, 1, 1, 2, 0, 2, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1,  //
                          1, 1, 1, 1, 0, 1, 1, 1, 1   //
                      },
                      {{4.0f, 0.0f, 3.0f}, {4.0f, 0.0f, 7.0f}}};

Scene::Scene(void) {}

Scene::Scene(Shader shader, Camera* camera, Renderer* renderer)
    : shader_id(shader.id),
      _camera(camera),
      _renderer(renderer),
      _meter_counter(-1),
      _paused(false) {
  this->floor_pool.push_back(floor_setup1);
  this->floor_pool.push_back(floor_setup2);
  this->floor_pool.push_back(floor_setup3);

  Texture* tex_ground = new Texture("textures/floor_black.png");
  Texture* tex_wall_white = new Texture("textures/white_wall.png");
  Texture* tex_wall_wood = new Texture("textures/wood_tex.png");

  this->floor_textures.push_back(tex_ground);
  this->wall_textures.push_back(tex_wall_white);
  this->wall_textures.push_back(tex_wall_wood);

  Model cube_model("models/cube.obj");
  this->vao_cube = new VAO(cube_model.vertices, cube_model.indices);

  Model teapot_model("models/teapot.obj");
  VAO* vao_teapot = new VAO(teapot_model.vertices, teapot_model.indices);
  GameObject* go_teapot = new GameObject(
      shader.id, vao_teapot, this->wall_textures[0], nullptr, nullptr, nullptr);
  obstacle_pool.push_back(go_teapot);

  GameObject* player =
      new GameObject(shader_id, nullptr, nullptr, new InputComponent(),
                     new PhysicsComponent(), nullptr);
  player->aabb_min = glm::vec3(-0.2f, 0.1f, -0.2f);
  player->aabb_max = glm::vec3(0.2f, 0.5f, 0.2f);
  player->is_collider = true;
  this->_player = player;
  world.entities.push_back(player);

  GameObject* floor1 =
      new GameObject(shader_id, nullptr, nullptr, nullptr, nullptr, nullptr);
  GameObject* floor2 =
      new GameObject(shader_id, nullptr, nullptr, nullptr, nullptr, nullptr);
  GameObject* floor3 =
      new GameObject(shader_id, nullptr, nullptr, nullptr, nullptr, nullptr);
  GameObject* floor4 =
      new GameObject(shader_id, nullptr, nullptr, nullptr, nullptr, nullptr);

  floor1->transform.position = glm::vec3(-4.0f, 0.5f, 0.0f);
  floor2->transform.position = glm::vec3(-4.0f, 0.5f, 9.0f);
  floor3->transform.position = glm::vec3(-4.0f, 0.5f, 18.0f);
  floor4->transform.position = glm::vec3(-4.0f, 0.5f, 27.0f);

  populateFloor(floor1, floor_pool[0]);
  populateFloor(floor2, floor_pool[1]);
  populateFloor(floor3, floor_pool[2]);
  populateFloor(floor4, floor_pool[0]);

  world.entities.push_back(floor1);
  world.entities.push_back(floor2);
  world.entities.push_back(floor3);
  world.entities.push_back(floor4);

  floors.push_back(floor1);
  floors.push_back(floor2);
  floors.push_back(floor3);
  floors.push_back(floor4);
}

Scene::Scene(Scene const& src) { *this = src; }

Scene::~Scene(void) {
  for (auto entity : world.entities) {
    delete entity;
  }
  for (auto tex : floor_textures) {
    delete tex;
  }
  for (auto tex : wall_textures) {
    delete tex;
  }
  for (auto tex : roof_textures) {
    delete tex;
  }
  for (auto obs : obstacle_pool) {
    delete obs;
  }
  delete vao_cube;
}

Scene& Scene::operator=(Scene const& rhs) {
  if (this != &rhs) {
    this->floors = rhs.floors;
    this->floor_textures = rhs.floor_textures;
    this->wall_textures = rhs.wall_textures;
    this->roof_textures = rhs.roof_textures;
    this->world = rhs.world;
    this->vao_cube = rhs.vao_cube;
    this->shader_id = rhs.shader_id;
    this->_camera = rhs._camera;
    this->_renderer = rhs._renderer;
    this->_player = rhs._player;
    this->_meter_counter = rhs._meter_counter;
    this->_paused = rhs._paused;
  }
  return (*this);
}

void Scene::update(InputHandler& inputHandler, float deltaTime) {
  if (this->_paused) return;
  if (this->floors.size() > 0 && this->floors.front()->transform.position.z -
                                         _player->transform.position.z <
                                     -12.0f) {
    GameObject* oldFloor = this->floors.front();
    world.entities.erase(
        std::remove_if(world.entities.begin(), world.entities.end(),
                       [&oldFloor](GameObject* go) {
                         if (go->parent != nullptr &&
                             (go->parent == oldFloor || go == oldFloor)) {
                           delete go;
                           return (true);
                         }
                         return (false);
                       }),
        world.entities.end());
    this->floors.pop_front();

    glm::vec3 floorPos = this->floors.back()->transform.position;
    floorPos.z += 9.0f;
    GameObject* newFloor =
        new GameObject(shader_id, nullptr, nullptr, nullptr, nullptr, nullptr);
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, floor_pool.size() - 1);
    int rand_nb = dist(mt);

    populateFloor(newFloor, floor_pool[dist(mt)]);
    newFloor->transform.position = floorPos;

    this->world.entities.push_back(newFloor);
    this->floors.push_back(newFloor);
  }
  world.update(inputHandler, deltaTime);

  if (_player->physicsComponent->has_collide) {
    this->_paused = true;
  }

  _camera->pos = _player->transform.position + glm::vec3(0.0f, 0.3f, -0.5f);

  _camera->update();
  _renderer->view = this->_camera->view;
  _renderer->proj = this->_camera->proj;

  this->_meter_counter += this->_player->positionRelative.z;
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

void Scene::populateFloor(GameObject* floor_ptr, const Floor& setup) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      int block_id = setup.setup[i * 9 + j];
      if (block_id == 0) {
        GameObject* mfloor =
            new GameObject(shader_id, vao_cube, this->floor_textures[0],
                           nullptr, nullptr, floor_ptr, glm::vec3(j, -1.0f, i));
        GameObject* mroof =
            new GameObject(shader_id, vao_cube, this->floor_textures[0],
                           nullptr, nullptr, floor_ptr, glm::vec3(j, 1.0f, i));
        world.entities.push_back(mfloor);
        world.entities.push_back(mroof);
      } else if (block_id != 0) {
        Texture* texture = nullptr;
        if (block_id - 1 < this->wall_textures.size()) {
          texture = this->wall_textures[block_id - 1];
        }
        GameObject* mwall =
            new GameObject(shader_id, vao_cube, texture, nullptr, nullptr,
                           floor_ptr, glm::vec3(j, 0.0f, i));
        world.entities.push_back(mwall);
      }
    }
  }

  for (auto entity : setup.entities) {
    GameObject* newEntity = new GameObject(*entity);
    newEntity->parent = floor_ptr;
    world.entities.push_back(newEntity);
  }

  // Populate floor with obstacle
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(0, 2);
  int rand_nb = dist(mt);
  std::cout << "rand_nb: " << rand_nb << std::endl;
  if (rand_nb == 0) {
    if (setup.obstacles_pos.size() > 0) {
      std::uniform_int_distribution<int> dist_pos(
          0, setup.obstacles_pos.size() - 1);
      glm::vec3 obstacle_pos = setup.obstacles_pos[dist_pos(mt)];
      obstacle_pos.y -= 0.4f;
      if (obstacle_pool.size() > 0) {
        std::cout << "new obstacles at: ";
        print_vec3(obstacle_pos);
        std::uniform_int_distribution<int> dist_obs(0,
                                                    obstacle_pool.size() - 1);
        GameObject* obstacle = obstacle_pool[dist_obs(mt)];
        /*GameObject* newObstacle = new GameObject(*obstacle);
        newObstacle->parent = floor_ptr;
        newObstacle->transform.position = obstacle_pos; */
        GameObject* newObstacle = new GameObject(
            shader_id, vao_cube, this->wall_textures[1], nullptr, nullptr,
            floor_ptr, obstacle_pos, glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.1f, 0.1f, 0.1f));
        newObstacle->is_collider = true;
        world.entities.push_back(newObstacle);
      }
    }
  }
}

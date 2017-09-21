#include "Scene.hpp"

const std::array<int, 81> setup_floor1 = {
    1, 1, 1, 1, 0, 1, 1, 1, 1,  //
    1, 1, 1, 1, 0, 1, 1, 1, 1,  //
    1, 1, 1, 1, 0, 1, 1, 1, 1,  //
    1, 1, 1, 1, 0, 1, 1, 1, 1,  //
    1, 1, 1, 1, 0, 1, 1, 1, 1,  //
    1, 1, 1, 1, 0, 1, 1, 1, 1,  //
    1, 1, 1, 1, 0, 1, 1, 1, 1,  //
    1, 1, 1, 1, 0, 1, 1, 1, 1,  //
    1, 1, 1, 1, 0, 1, 1, 1, 1   //
};

const std::array<int, 81> setup_floor2 = {
    1, 1, 1, 1, 0, 1, 1, 1, 1,  //
    1, 1, 1, 1, 0, 1, 1, 1, 1,  //
    1, 1, 1, 0, 0, 0, 1, 1, 1,  //
    1, 1, 1, 0, 0, 0, 1, 1, 1,  //
    1, 1, 1, 0, 0, 0, 1, 1, 1,  //
    1, 1, 1, 0, 0, 0, 1, 1, 1,  //
    1, 1, 1, 0, 0, 0, 1, 1, 1,  //
    1, 1, 1, 1, 0, 1, 1, 1, 1,  //
    1, 1, 1, 1, 0, 1, 1, 1, 1   //
};

Scene::Scene(void) {}

Scene::Scene(Shader shader, Camera* camera, Renderer* renderer, VAO* cube)
    : shader(shader.id),
      _camera(camera),
      _renderer(renderer),
      _meter_counter(-1) {
  this->vao_cube = cube;
  GameObject* player =
      new GameObject(shader.id, nullptr, nullptr, new InputComponent(),
                     new PhysicsComponent(), nullptr);
  player->aabb_min = glm::vec3(-0.2f, 0.1f, -0.2f);
  player->aabb_max = glm::vec3(0.2f, 0.5f, 0.2f);
  player->is_collider = true;
  this->_player = player;
  world.entities.push_back(player);
}

void Scene::init() {
  GameObject* floor1 =
      new GameObject(shader, nullptr, nullptr, nullptr, nullptr, nullptr);
  GameObject* floor2 =
      new GameObject(shader, nullptr, nullptr, nullptr, nullptr, nullptr);
  GameObject* floor3 =
      new GameObject(shader, nullptr, nullptr, nullptr, nullptr, nullptr);
  GameObject* floor4 =
      new GameObject(shader, nullptr, nullptr, nullptr, nullptr, nullptr);

  floor1->transform.position = glm::vec3(-4.0f, 0.5f, 0.0f);
  print_vec3(floor1->transform.position);
  floor2->transform.position = glm::vec3(-4.0f, 0.5f, 9.0f);
  floor3->transform.position = glm::vec3(-4.0f, 0.5f, 18.0f);
  floor4->transform.position = glm::vec3(-4.0f, 0.5f, 27.0f);

  populateFloor(floor1, setup_floor1);
  populateFloor(floor2, setup_floor2);
  populateFloor(floor3, setup_floor1);
  populateFloor(floor4, setup_floor1);

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
}

Scene& Scene::operator=(Scene const& rhs) {
  if (this != &rhs) {
  }
  return (*this);
}

void Scene::update(InputHandler& inputHandler, float deltaTime) {
  if (this->floors.size() > 0 && this->floors.front()->transform.position.z -
                                         _player->transform.position.z <
                                     -9.0f) {
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
    print_vec3(floorPos);
    floorPos.z += 9.0f;
    // TODO: rand floor
    GameObject* newFloor =
        new GameObject(shader, nullptr, nullptr, nullptr, nullptr, nullptr);
    populateFloor(newFloor, setup_floor1);
    newFloor->transform.position = floorPos;

    this->world.entities.push_back(newFloor);
    this->floors.push_back(newFloor);
  }
  world.update(inputHandler, deltaTime);

  _camera->pos = _player->transform.position;
  _camera->pos.y += 0.3f;

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
  drawUI();
}

void Scene::drawUI() {
  this->_renderer->renderText(
      this->_renderer->getScreenWidth() - 100.0f,
      this->_renderer->getScreenHeight() - 50.0f, 1.0f,
      std::to_string(static_cast<int>(this->_meter_counter)),
      glm::vec3(0.0f, 0.0f, 0.0f));
}

void Scene::populateFloor(GameObject* floor_ptr, std::array<int, 81> setup) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      int block_id = setup[i * 9 + j];
      if (block_id == 0) {
        GameObject* mfloor =
            new GameObject(shader, vao_cube, this->floor_textures[0], nullptr,
                           nullptr, floor_ptr, glm::vec3(j, -1.0f, i));
        GameObject* mroof =
            new GameObject(shader, vao_cube, this->floor_textures[0], nullptr,
                           nullptr, floor_ptr, glm::vec3(j, 1.0f, i));
        world.entities.push_back(mfloor);
        world.entities.push_back(mroof);
      } else if (block_id != 0) {
        GameObject* mwall =
            new GameObject(shader, vao_cube, this->wall_textures[0], nullptr,
                           nullptr, floor_ptr, glm::vec3(j, 0.0f, i));
        world.entities.push_back(mwall);
      }
    }
  }
}

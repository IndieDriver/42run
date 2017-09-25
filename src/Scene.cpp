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
                      {{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 7.0f}}};

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
                      {{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 7.0f}}};

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
                      {{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 7.0f}}};

Scene::Scene(void) {}

Scene::Scene(Shader shader, Camera* camera, Renderer* renderer, VAO* cube)
    : shader(shader.id),
      _camera(camera),
      _renderer(renderer),
      _meter_counter(-1) {
  Texture* tex_ground = new Texture("textures/floor_black.png");
  Texture* tex_wall_white = new Texture("textures/white_wall.png");
  Texture* tex_wall_wood = new Texture("textures/wood_tex.png");

  this->floor_textures.push_back(tex_ground);
  this->wall_textures.push_back(tex_wall_white);
  this->wall_textures.push_back(tex_wall_wood);

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
}

Scene& Scene::operator=(Scene const& rhs) {
  if (this != &rhs) {
  }
  return (*this);
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
  floor2->transform.position = glm::vec3(-4.0f, 0.5f, 9.0f);
  floor3->transform.position = glm::vec3(-4.0f, 0.5f, 18.0f);
  floor4->transform.position = glm::vec3(-4.0f, 0.5f, 27.0f);

  populateFloor(floor1, floor_setup1);
  populateFloor(floor2, floor_setup2);
  populateFloor(floor3, floor_setup3);
  populateFloor(floor4, floor_setup1);

  world.entities.push_back(floor1);
  world.entities.push_back(floor2);
  world.entities.push_back(floor3);
  world.entities.push_back(floor4);

  floors.push_back(floor1);
  floors.push_back(floor2);
  floors.push_back(floor3);
  floors.push_back(floor4);
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
    floorPos.z += 9.0f;
    // TODO: rand floor
    GameObject* newFloor =
        new GameObject(shader, nullptr, nullptr, nullptr, nullptr, nullptr);
    populateFloor(newFloor, floor_setup1);
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

void Scene::populateFloor(GameObject* floor_ptr, const Floor& setup) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      int block_id = setup.setup[i * 9 + j];
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
        Texture* texture = nullptr;
        if (block_id - 1 < this->wall_textures.size()) {
          texture = this->wall_textures[block_id - 1];
        }
        GameObject* mwall =
            new GameObject(shader, vao_cube, texture, nullptr, nullptr,
                           floor_ptr, glm::vec3(j, 0.0f, i));
        world.entities.push_back(mwall);
      }
    }
  }
}

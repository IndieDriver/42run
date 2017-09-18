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

Scene::Scene(Shader shader, Renderer* renderer, VAO* cube)
    : _renderer(renderer), _meter_counter(-1) {
  this->vao_cube = cube;
  GameObject* player =
      new GameObject(shader.id, nullptr, nullptr, new InputComponent(),
                     new PhysicsComponent());
  world.entities.push_back(player);
}

void Scene::init() {
  GameObject* floor1 =
      new GameObject(shader, vao_cube, nullptr, nullptr, nullptr);
  GameObject* floor2 =
      new GameObject(shader, vao_cube, nullptr, nullptr, nullptr);
  GameObject* floor3 =
      new GameObject(shader, vao_cube, nullptr, nullptr, nullptr);
  GameObject* floor4 =
      new GameObject(shader, vao_cube, nullptr, nullptr, nullptr);

  // Floor* floor1 = new Floor(setup_floor1);
  // Floor* floor2 = new Floor(setup_floor2);
  // Floor* floor3 = new Floor(setup_floor1);
  // Floor* floor4 = new Floor(setup_floor1);
  populateFloor(floor1, setup_floor1);
  populateFloor(floor2, setup_floor2);
  populateFloor(floor3, setup_floor1);
  populateFloor(floor4, setup_floor1);

  floor1->setTransform(glm::vec3(-4.0f, 0.5f, 0.0f));
  floor2->setTransform(glm::vec3(-4.0f, 0.5f, 9.0f));
  floor3->setTransform(glm::vec3(-4.0f, 0.5f, 18.0f));
  floor4->setTransform(glm::vec3(-4.0f, 0.5f, 27.0f));

  world.entities.push_back(floor1);
  world.entities.push_back(floor2);
  world.entities.push_back(floor3);
  world.entities.push_back(floor4);
  /* floors.push_back(floor1);
   floors.push_back(floor2);
   floors.push_back(floor3);
   floors.push_back(floor4); */
}

Scene::Scene(Scene const& src) { *this = src; }

Scene::~Scene(void) {
  for (auto flr : floors) {
    delete flr;
  }
}

Scene& Scene::operator=(Scene const& rhs) {
  if (this != &rhs) {
  }
  return (*this);
}

/*
void Player::update(std::array<bool, 1024> keys, float deltaTime) {
  glm::vec3 backupPosition = this->position;
  if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
    this->position.x += 1.0f * deltaTime;
  }
  if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
    this->position.x -= 1.0f * deltaTime;
  }
  if (keys[GLFW_KEY_SPACE]) {
    if (this->position.y == 0.0f) {
      this->velocity.y = 0.7f;
    }
  }
  this->speed += 0.1f * deltaTime;
  this->position.z += log(speed) * 2.0f * deltaTime;
  this->velocity.y -= 0.81f * deltaTime;
  this->position += velocity * deltaTime;

  this->position.y = glm::clamp(this->position.y, 0.0f, 1.0f);
  this->velocity.y = glm::clamp(this->velocity.y, -10.0f, 3.0f);
  this->offsetSinceLastFrame = backupPosition - this->position;
} */

void Scene::update(InputHandler& inputHandler, float deltaTime) {
  world.update(inputHandler, deltaTime);
  /*
  this->_player->update(keys, deltaTime);

  if (this->floors[0]->getPosition().z < -9.0f) {
    // front floor is behind us, delete it and stack a new one
    delete this->floors[0];
    this->floors.pop_front();
    // TODO: rand floor
    glm::vec3 floorPos = this->floors.back()->getPosition();
    floorPos.z += 9.0f;
    Floor* newFloor = new Floor(setup_floor1);
    populateFloor(newFloor);
    newFloor->setTransform(floorPos);
    this->floors.push_back(newFloor);
  }
  glm::vec3 pos = this->floors[0]->getPosition();
  for (auto flr : this->floors) {
    flr->move(_player.offsetSinceLastFrame);
  }
  this->_meter_counter -= _player.offsetSinceLastFrame.z; */
}

void Scene::draw() {
  for (const auto entity : world.entities) {
    this->_renderer->addRenderAttrib(entity->getRenderAttrib());
  }
  /*
  for (const auto& flr : floors) {
    for (const auto& model : flr->models) {
      this->_renderer->addRenderAttrib(model.getRenderAttrib());
    }
  } */
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
                           nullptr, glm::vec3(j, -1.0f, i));
        // mfloor->setTransform(glm::vec3(j, -1.0f, i));
        // mfloor->setTexture(this->floor_textures[0]);
        floor_ptr->childrens.push_back(mfloor);

        GameObject* mroof =
            new GameObject(shader, vao_cube, this->floor_textures[0], nullptr,
                           nullptr, glm::vec3(j, 1.0f, i));
        // GameObject* mroof = new GameObject(*cubeModel);
        // mroof->setTransform(glm::vec3(j, 1.0f, i));
        // mroof->setTexture(this->floor_textures[0]);
        floor_ptr->childrens.push_back(mroof);
      } else if (block_id != 0) {
        GameObject* mwall =
            new GameObject(shader, vao_cube, this->floor_textures[0], nullptr,
                           nullptr, glm::vec3(j, 1.0f, i));
        // GameObject* mwall = new GameObject(*cubeModel);
        // mwall->setTransform(glm::vec3(j, 0.0f, i));
        // mwall->setTexture(this->wall_textures[0]);
        floor_ptr->childrens.push_back(mwall);
      }
    }
  }
}

Floor::Floor(std::array<int, 81> init) {
  setup = init;
  this->_position = glm::vec3(0.0f, 0.0f, 0.0f);
  this->_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  this->_scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Floor::move(glm::vec3 pos) {
  this->_position += pos;
  updateTransform();
}

void Floor::setTransform(glm::vec3 position, glm::vec3 rotation,
                         glm::vec3 scale) {
  this->_position = position;
  this->_rotation = rotation;
  this->_scale = scale;
  updateTransform();
}

void Floor::updateTransform() {
  glm::mat4 mat_translation = glm::translate(_position);
  glm::mat4 mat_rotation =
      glm::eulerAngleYXZ(_rotation.y, _rotation.x, _rotation.z);
  glm::mat4 mat_scale = glm::scale(_scale);
  this->transform = mat_translation * mat_rotation * mat_scale;
  for (auto& model : models) {
    model.setTransform(this->transform * model.getTransform());
  }
}

glm::vec3 Floor::getPosition() { return (_position); }

glm::vec3 Floor::getRotation() { return (_rotation); }

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

Scene::Scene(Shader shader, Renderer* renderer) : _renderer(renderer) {}

void Scene::init() {
  Floor* floor1 = new Floor(setup_floor1);
  Floor* floor2 = new Floor(setup_floor2);

  populateFloor(floor1, glm::vec3(4.0f, 0.0f, 9.0f));
  populateFloor(floor2, glm::vec3(4.0f, 0.0f, 18.0f));
  floors.push_back(floor1);
  floors.push_back(floor2);
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

void Scene::draw() {
  for (const auto& flr : floors) {
    for (const auto& model : flr->models) {
      this->_renderer->draw(model.getRenderAttrib());
    }
  }
}

void Scene::populateFloor(Floor* floor_ptr, glm::vec3 startPosition) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      int block_id = floor_ptr->setup[i * 9 + j];
      if (block_id == 0) {
        Model mfloor(*cubeModel);
        mfloor.setTransform(glm::vec3(
            startPosition.x - j, startPosition.y - 1.0f, startPosition.z - i));
        mfloor.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
        floor_ptr->models.push_back(mfloor);

        Model mroof(*cubeModel);
        mroof.setTransform(glm::vec3(
            startPosition.x - j, startPosition.y + 1.0f, startPosition.z - i));
        mroof.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
        floor_ptr->models.push_back(mroof);
      } else if (block_id != 0) {
        Model mwall(*cubeModel);
        mwall.setTransform(glm::vec3(startPosition.x - j, startPosition.y,
                                     startPosition.z - i));
        mwall.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
        floor_ptr->models.push_back(mwall);
      }
    }
  }
}

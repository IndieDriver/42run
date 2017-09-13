#include "Scene.hpp"

const int setup_floor1[9][9] = {
    {1, 1, 1, 1, 0, 1, 1, 1, 1}, {1, 1, 1, 1, 0, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 1, 1, 1, 1}, {1, 1, 1, 1, 0, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 1, 1, 1, 1}, {1, 1, 1, 1, 0, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 1, 1, 1, 1}, {1, 1, 1, 1, 0, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 1, 1, 1, 1}};

Scene::Scene(void) {}

Scene::Scene(Shader shader, Renderer* renderer) : _renderer(renderer) {}

Scene::Scene(Scene const& src) { *this = src; }

Scene::~Scene(void) {}

Scene& Scene::operator=(Scene const& rhs) {
  if (this != &rhs) {
  }
  return (*this);
}

void Scene::draw() {
  for (const auto& model : models) {
    this->_renderer->draw(model.getRenderAttrib());
  }
}

void Scene::loadFloor(glm::vec3 startPosition) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      int block_id = setup_floor1[i][j];
      if (block_id == 0) {
        Model block(*cubeModel);
        block.setTransform(
            glm::vec3(startPosition.x - j, -1.0f, startPosition.y - i));
        block.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
        models.push_back(block);
      } else if (block_id != 0) {
        Model block(*cubeModel);
        block.setTransform(
            glm::vec3(startPosition.x - j, 0.0f, startPosition.y - i));
        block.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
        models.push_back(block);
      }
    }
  }
}

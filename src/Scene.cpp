#include "Scene.hpp"

Scene::Scene(void) {}

Scene::Scene(Renderer* renderer) : _renderer(renderer) {}

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

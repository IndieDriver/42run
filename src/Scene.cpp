#include "Scene.hpp"

Scene::Scene(void) {}

Scene::Scene(Scene const& src) { *this = src; }

Scene::~Scene(void) {}

Scene& Scene::operator=(Scene const& rhs) {
  if (this != &rhs) {
  }
  return (*this);
}

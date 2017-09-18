#pragma once
#include <array>
#include "Renderer.hpp"
#include "env.hpp"
#include "run.hpp"

class GameObject;
class PhysicsComponent;
class InputComponent;

class World {
 public:
  std::vector<GameObject*> entities;
  float deltaTime;  // time since last frame
  void update(InputHandler& inputHandler, float deltaTime);
  bool collide(GameObject& gameObject);
};

class GameObject {
 public:
  GameObject(GLuint shader, VAO* vao, Texture* texture,
             InputComponent* inputComponent, PhysicsComponent* physicsComponent,
             glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
             glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f),
             glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
  GameObject(GameObject const& src);
  virtual ~GameObject(void);
  GameObject& operator=(GameObject const& rhs);
  const RenderAttrib getRenderAttrib() const;
  void update(World& world, InputHandler& inputHandler);
  void add(glm::mat4 transform);
  void setTransform(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
  void setTransform(glm::mat4 transform);
  glm::mat4 getTransform();
  glm::vec3 getAABBmin();
  glm::vec3 getAABBmax();
  void updateTransform();

  void setTexture(Texture* texture);
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
  std::vector<GameObject*> childrens;

 private:
  GameObject(void);
  RenderAttrib _renderAttrib;
  InputComponent* inputComponent;
  PhysicsComponent* physicsComponent;
  glm::vec3 _aabb_min;  // Don't multiply by transform
  glm::vec3 _aabb_max;  // Don't multiply by transform
};

class InputComponent {
 public:
  InputComponent(void);
  InputComponent(InputComponent const& src);
  virtual ~InputComponent(void);
  InputComponent& operator=(InputComponent const& rhs);
  void update(GameObject& gameObject, World& world, InputHandler& inputHandler,
              PhysicsComponent* physicsComponent);
};

class PhysicsComponent {
 public:
  PhysicsComponent(void);
  PhysicsComponent(PhysicsComponent const& src);
  virtual ~PhysicsComponent(void);
  PhysicsComponent& operator=(PhysicsComponent const& rhs);
  void update(GameObject& gameObject, World& world);

  glm::vec3 velocity;
  float speed;
};

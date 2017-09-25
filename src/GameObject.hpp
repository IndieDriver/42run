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

struct Transform {
  glm::vec3 position = {0.0f, 0.0f, 0.0f};
  glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
  glm::vec3 scale = {1.0f, 1.0f, 1.0f};

  Transform() {
    this->_old_position = position;
    this->_old_rotation = rotation;
    this->_old_scale = scale;
    this->_model = glm::mat4(1.0f);
  }

  glm::mat4 toModelMatrix() {
    // only update when one attribute has changed
    if (position != _old_position || rotation != _old_rotation ||
        scale != _old_scale) {
      glm::mat4 mat_translation = glm::translate(position);
      glm::mat4 mat_rotation =
          glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
      glm::mat4 mat_scale = glm::scale(scale);
      this->_model = mat_translation * mat_rotation * mat_scale;
      this->_old_position = position;
      this->_old_rotation = rotation;
      this->_old_scale = scale;
    }
    return (this->_model);
  }

 private:
  glm::mat4 _model;
  glm::vec3 _old_position;
  glm::vec3 _old_rotation;
  glm::vec3 _old_scale;
};

class GameObject {
 public:
  GameObject(GLuint shader, VAO* vao, Texture* texture,
             InputComponent* inputComponent, PhysicsComponent* physicsComponent,
             GameObject* parent, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
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
  glm::mat4 getWorldTransform();
  glm::vec4 getWorldPosition();

  void setTexture(Texture* texture);
  Transform transform;
  GameObject* parent;
  glm::vec3 positionRelative = {0.0f, 0.0f, 0.0f};

  glm::vec3 aabb_min;  // model space
  glm::vec3 aabb_max;  // model space
  bool is_collider;
  InputComponent* inputComponent;
  PhysicsComponent* physicsComponent;

 private:
  GameObject(void);
  RenderAttrib _renderAttrib;
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

  glm::vec3 velocity = {0.0f, 0.0f, 0.0f};
  float speed;
  bool is_alive;  // ikr
};

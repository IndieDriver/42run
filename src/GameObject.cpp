#include "GameObject.hpp"

void World::update(InputHandler& inputHandler, float deltaT) {
  this->deltaTime = deltaT;
  for (auto& entity : entities) {
    entity->update(*this, inputHandler);
  }
}

bool World::collide(GameObject& gameObject) {
  glm::vec4 aabb1_min_wspace =
      gameObject.getWorldTransform() * glm::vec4(gameObject.aabb_min, 1.0f);
  glm::vec4 aabb1_max_wspace =
      gameObject.getWorldTransform() * glm::vec4(gameObject.aabb_max, 1.0f);

  for (auto entity : entities) {
    if (entity->is_collider && entity != &gameObject) {
      glm::mat4 entity_model_matrix = entity->getWorldTransform();
      glm::vec4 aabb2_min_wspace =
          entity_model_matrix * glm::vec4(entity->aabb_min, 1.0f);
      glm::vec4 aabb2_max_wspace =
          entity_model_matrix * glm::vec4(entity->aabb_max, 1.0f);

      bool isColliding = (aabb1_max_wspace.x >= aabb2_min_wspace.x &&
                          aabb1_min_wspace.x <= aabb2_max_wspace.x) &&
                         (aabb1_max_wspace.y >= aabb2_min_wspace.y &&
                          aabb1_min_wspace.y <= aabb2_max_wspace.y) &&
                         (aabb1_max_wspace.z >= aabb2_min_wspace.z &&
                          aabb1_min_wspace.z <= aabb2_max_wspace.z);
      if (isColliding) {
        return (true);
      }
    }
  }
  return (false);
}

GameObject::GameObject(void){};

GameObject::GameObject(GLuint shader, VAO* vao, Texture* texture,
                       InputComponent* inputComponent,
                       PhysicsComponent* physicsComponent, GameObject* parent,
                       glm::vec3 pos, glm::vec3 rot, glm::vec3 sca)
    : parent(parent),
      is_collider(false),
      inputComponent(inputComponent),
      physicsComponent(physicsComponent) {
  this->transform.position = pos;
  this->transform.rotation = rot;
  this->transform.scale = sca;
  this->_renderAttrib.shader = shader;
  this->_renderAttrib.vao = vao;
  this->_renderAttrib.transforms.push_back(glm::mat4(1.0f));
  this->_renderAttrib.texture = texture;
  if (vao != nullptr) {
    this->aabb_min = vao->aabb_min;
    this->aabb_max = vao->aabb_max;
  } else {
    this->aabb_min = glm::vec3(0.0f, 0.0f, 0.0f);
    this->aabb_max = glm::vec3(0.0f, 0.0f, 0.0f);
  }
}

GameObject::GameObject(GameObject const& src) { *this = src; }

GameObject::~GameObject(void) {}

GameObject& GameObject::operator=(GameObject const& rhs) {
  if (this != &rhs) {
    this->_renderAttrib = rhs._renderAttrib;
    this->_renderAttrib.shader = rhs._renderAttrib.shader;
    this->_renderAttrib.vao = rhs._renderAttrib.vao;
    this->_renderAttrib.transforms = rhs._renderAttrib.transforms;
    this->_renderAttrib.texture = rhs._renderAttrib.texture;
    this->inputComponent = rhs.inputComponent;
    this->physicsComponent = rhs.physicsComponent;
    this->transform.position = rhs.transform.position;
    this->transform.rotation = rhs.transform.rotation;
    this->transform.scale = rhs.transform.scale;
    this->parent = rhs.parent;
    this->positionRelative = rhs.positionRelative;
    this->aabb_min = rhs.aabb_min;
    this->aabb_max = rhs.aabb_max;
    this->is_collider = rhs.is_collider;
  }
  return (*this);
}

void GameObject::update(World& world, InputHandler& inputHandler) {
  glm::vec3 oldPosition = this->transform.position;
  if (inputComponent != nullptr) {
    inputComponent->update(*this, world, inputHandler, this->physicsComponent);
  }
  if (physicsComponent != nullptr) {
    physicsComponent->update(*this, world);
  }
  this->_renderAttrib.transforms[0] = this->getWorldTransform();
  this->positionRelative = this->transform.position - oldPosition;
}

glm::mat4 GameObject::getWorldTransform() {
  glm::mat4 worldTransform = transform.toModelMatrix();
  if (this->parent != nullptr) {
    worldTransform = this->parent->getWorldTransform() * worldTransform;
  }
  return (worldTransform);
}

const RenderAttrib GameObject::getRenderAttrib() const {
  return (this->_renderAttrib);
}

void GameObject::setTexture(Texture* texture) {
  this->_renderAttrib.texture = texture;
}

PhysicsComponent::PhysicsComponent(void) : speed(5.0f){};

PhysicsComponent::~PhysicsComponent(void){};

PhysicsComponent::PhysicsComponent(PhysicsComponent const& src) { *this = src; }

PhysicsComponent& PhysicsComponent::operator=(PhysicsComponent const& rhs) {
  if (this != &rhs) {
    this->velocity = rhs.velocity;
    this->speed = rhs.speed;
    this->has_collide = rhs.has_collide;
  }
  return (*this);
}

void PhysicsComponent::update(GameObject& gameObject, World& world) {
  has_collide = false;  // Reset every frame
  glm::vec3 backupPosition = gameObject.transform.position;
  this->speed += 0.1f * world.deltaTime;
  gameObject.transform.position.z += log(this->speed) * 2.0f * world.deltaTime;
  this->velocity.y -= 0.81f * world.deltaTime;
  gameObject.transform.position += velocity * world.deltaTime;

  gameObject.transform.position.y =
      glm::clamp(gameObject.transform.position.y, 0.0f, 1.0f);
  this->velocity.y = glm::clamp(this->velocity.y, -10.0f, 3.0f);
  gameObject.transform.position.x =
      glm::clamp(gameObject.transform.position.x, -0.29f, 0.29f);
  if (fabs(gameObject.transform.position.x) == 0.29f) {
    // Lateral wall was hit, nullify lateral velocity
    velocity.x = 0.0f;
  }
  if (world.collide(gameObject)) {
    this->has_collide = true;
    std::cout << "collide" << std::endl;
    // end game ???
  }
}

InputComponent::InputComponent(void){};

InputComponent::~InputComponent(void){};

InputComponent::InputComponent(InputComponent const& src) { *this = src; }

InputComponent& InputComponent::operator=(InputComponent const& rhs) {
  if (this != &rhs) {
  }
  return (*this);
}

void InputComponent::update(GameObject& gameObject, World& world,
                            InputHandler& inputHandler,
                            PhysicsComponent* physicsComponent) {
  if (physicsComponent != nullptr) {
    if (inputHandler.keys[GLFW_KEY_A] || inputHandler.keys[GLFW_KEY_LEFT]) {
      physicsComponent->velocity.x += 1.0f * world.deltaTime;
    }
    if (inputHandler.keys[GLFW_KEY_D] || inputHandler.keys[GLFW_KEY_RIGHT]) {
      physicsComponent->velocity.x -= 1.0f * world.deltaTime;
    }
    if (inputHandler.keys[GLFW_KEY_SPACE]) {
      if (gameObject.transform.position.y == 0.0f) {
        physicsComponent->velocity.y = 0.7f;
      }
    }
  }
}

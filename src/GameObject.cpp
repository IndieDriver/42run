#include "GameObject.hpp"

void World::update(InputHandler& inputHandler, float deltaT) {
  this->deltaTime = deltaT;
  for (auto& entity : entities) {
    entity->update(*this, inputHandler);
  }
}

bool World::collide(GameObject& gameObject) {
  for (auto entity : entities) {
    if (entity->is_collider && entity != &gameObject) {
      if (glm::distance(gameObject.transform.position, entity->transform.position) < 1.3f) {
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
}

GameObject::GameObject(GameObject const& src) { *this = src; }

GameObject::~GameObject(void) {
  if (this->inputComponent != nullptr) {
    delete this->inputComponent;
  }
  if (this->physicsComponent != nullptr) {
    delete this->physicsComponent;
  }
}

GameObject& GameObject::operator=(GameObject const& rhs) {
  if (this != &rhs) {
    this->_renderAttrib = rhs._renderAttrib;
    this->_renderAttrib.shader = rhs._renderAttrib.shader;
    this->_renderAttrib.vao = rhs._renderAttrib.vao;
    this->_renderAttrib.transforms = rhs._renderAttrib.transforms;
    this->_renderAttrib.texture = rhs._renderAttrib.texture;
    this->inputComponent =
        rhs.inputComponent ? new InputComponent(*rhs.inputComponent) : nullptr;
    this->physicsComponent = rhs.physicsComponent
                                 ? new PhysicsComponent(*rhs.physicsComponent)
                                 : nullptr;
    this->transform.position = rhs.transform.position;
    this->transform.rotation = rhs.transform.rotation;
    this->transform.scale = rhs.transform.scale;
    this->parent = rhs.parent;
    this->positionRelative = rhs.positionRelative;
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
  this->speed += 0.2f * world.deltaTime;
  gameObject.transform.position.z += log(this->speed) * 5.0f * world.deltaTime;
  this->velocity.y -= 0.81f * 4.0f * world.deltaTime;
  gameObject.transform.position.y += velocity.y * world.deltaTime;

  gameObject.transform.position.y =
      glm::clamp(gameObject.transform.position.y, 0.0f, 5.0f);
  this->velocity.y = glm::clamp(this->velocity.y, -10.0f, 3.0f);

  glm::vec3 target = gameObject.transform.position;
  if (gameObject.inputComponent->targetRail == Rail::left) {
    target.x = 1.25f;
  } else if (gameObject.inputComponent->targetRail == Rail::center) {
    target.x = 0.0f;
  } else if (gameObject.inputComponent->targetRail == Rail::right) {
    target.x = -1.25f;
  }
  gameObject.transform.position +=
      (target - gameObject.transform.position) * world.deltaTime * 5.0f;
  if (world.collide(gameObject)) {
    this->has_collide = true;
    /* std::cout << "collide" << std::endl; */
    // end game ???
  }
}

InputComponent::InputComponent(void) : targetRail(Rail::center){};

InputComponent::~InputComponent(void){};

InputComponent::InputComponent(InputComponent const& src) { *this = src; }

InputComponent& InputComponent::operator=(InputComponent const& rhs) {
  if (this != &rhs) {
    this->targetRail = rhs.targetRail;
  }
  return (*this);
}

void InputComponent::update(GameObject& gameObject, World& world,
                            InputHandler& inputHandler,
                            PhysicsComponent* physicsComponent) {
  if (physicsComponent != nullptr) {
    bool left = false;
    bool right = false;
    if (inputHandler.keys[GLFW_KEY_A] || inputHandler.keys[GLFW_KEY_LEFT]) {
      left = true;
      targetRail = Rail::left;
    }
    if (inputHandler.keys[GLFW_KEY_D] || inputHandler.keys[GLFW_KEY_RIGHT]) {
      right = true;
      targetRail = Rail::right;
    }
    if ((right && left) || (!right && !left)) {
      targetRail = Rail::center;
    }
    if (inputHandler.keys[GLFW_KEY_SPACE]) {
      if (gameObject.transform.position.y == 0.0f) {
        physicsComponent->velocity.y = 2.8f;
      }
    }
  }
}

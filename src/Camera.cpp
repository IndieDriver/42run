#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 targetPosition, int w, int h) {
  pos = position;

  width = w;
  height = h;

  (void)targetPosition;
  mouseXpos = static_cast<float>(width) / 2.0f;
  mouseYpos = static_cast<float>(height) / 2.0f;

  glm::vec3 direction = glm::normalize(targetPosition - position);
  verAngle = asinf(direction.y);
  horAngle = atan2(direction.x, direction.z);
  proj = glm::perspective(
      45.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f,
      100.0f);
  update();
}

void Camera::update() {
  double currentTime = glfwGetTime();
  deltaTime = static_cast<float>(currentTime - lastTime);
  lastTime = static_cast<float>(currentTime);
  if (mouseMoved) {
    horAngle += 0.5f * deltaTime * (width / 2.0f - mouseXpos);
    verAngle += 0.5f * deltaTime * (height / 2.0f - mouseYpos);
    mouseMoved = false;
  }
  dir = glm::vec3(cos(verAngle) * sin(horAngle), sin(verAngle),
                  cos(verAngle) * cos(horAngle));
  dir = glm::normalize(dir);
  right =
      glm::vec3(sin(horAngle - 3.14f / 2.0f), 0, cos(horAngle - 3.14f / 2.0f));
  right = glm::normalize(right);
  up = glm::cross(right, dir);
  up = glm::normalize(up);
  view = glm::lookAt(pos, dir + pos, up);
}

glm::mat4 getMVP(glm::mat4 model, glm::mat4 view, glm::mat4 proj) {
  return (model * view * proj);
}

void Camera::queryInput(std::array<bool, 1024> keys, float mouse_x,
                        float mouse_y) {
  if (keys[GLFW_KEY_LEFT_SHIFT]) {
    speed = 9.0f;
  } else {
    speed = 3.0f;
  }
  if (keys[GLFW_KEY_UP] || keys[GLFW_KEY_W]) {
    glm::vec3 tmp = dir * speed * deltaTime;
    pos = pos + tmp;
  }
  if (keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_S]) {
    glm::vec3 tmp = dir * speed * deltaTime;
    pos = pos - tmp;
  }
  if (keys[GLFW_KEY_RIGHT] || keys[GLFW_KEY_D]) {
    glm::vec3 right = glm::cross(up, dir);
    glm::vec3 tmp = right * speed * deltaTime;
    pos = pos - tmp;
  }
  if (keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_A]) {
    glm::vec3 right = glm::cross(up, dir);
    glm::vec3 tmp = right * speed * deltaTime;
    pos = pos + tmp;
  }
  /* if (inputHandler->mouseDisabled) return; */
  /*
  if (mouse_x != mouseXpos || mouse_y != mouseYpos) {
    mouseXpos = mouse_x;
    mouseYpos = mouse_y;
    mouseMoved = true;
  } */
}

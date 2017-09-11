#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 targetPosition, int w, int h)
{
    pos = position;

    width = w;
    height = h;

    (void)targetPosition;
    mouseXpos = static_cast<float>(width) / 2.0f;
    mouseYpos = static_cast<float>(height) / 2.0f;

    verAngle = 0.0f;
    horAngle = 180.0f * (M_PI / 180.0f);
    proj = glm::perspective(45.0f,
		static_cast<float>(width) / static_cast<float>(height),
		1.0f,
		1000.0f);
    update();
}

void Camera::update(){
    queryInput();
    double currentTime = glfwGetTime();
    deltaTime = static_cast<float>(currentTime - lastTime);
    lastTime = static_cast<float>(currentTime);
    if (mouseMoved) {
	horAngle += 0.5f * deltaTime * (width / 2.0f - mouseXpos);
	verAngle += 0.5f * deltaTime * (height / 2.0f - mouseYpos);
	mouseMoved = false;
    }
    dir = glm::vec3(cos(verAngle) * sin(horAngle),
	    sin(verAngle),
	    cos(verAngle) * cos(horAngle));
    dir = glm::normalize(dir);
    right = glm::vec3(sin(horAngle - 3.14f/2.0f),
	    0,
	    cos(horAngle - 3.14f/2.0f));
    right = glm::normalize(right);
    up = glm::cross(right, dir);
    up = glm::normalize(up);
    view = glm::lookAt(pos, dir + pos, up);
}

glm::mat4 getMVP(glm::mat4 model, glm::mat4 view, glm::mat4 proj){
    return (model * view * proj);
}

void Camera::queryInput() {
    /*
    if (inputHandler == nullptr)
	return;
    if (inputHandler->keybrDisabled)
	return;
    if (inputHandler->keys[GLFW_KEY_LEFT_SHIFT]) {
	speed = 9.0f;
    } else {
	speed = 3.0f;
    }
    if (inputHandler->keys[GLFW_KEY_UP] || inputHandler->keys[GLFW_KEY_W]){
	Vec3 tmp = dir * speed * deltaTime;
	pos = pos + tmp;
    }
    if (inputHandler->keys[GLFW_KEY_DOWN] || inputHandler->keys[GLFW_KEY_S]){
	Vec3 tmp = dir * speed * deltaTime;
	pos = pos - tmp;
    }
    if (inputHandler->keys[GLFW_KEY_RIGHT] || inputHandler->keys[GLFW_KEY_D]){
	Vec3 right = up.cross(dir);
	Vec3 tmp = right * speed * deltaTime;
	pos = pos - tmp;
    }
    if (inputHandler->keys[GLFW_KEY_LEFT] || inputHandler->keys[GLFW_KEY_A]){
	Vec3 right = up.cross(dir);
	Vec3 tmp = right * speed * deltaTime;
	pos = pos + tmp;
    }
    if (inputHandler->mouseDisabled)
	return;
    if (inputHandler->mousex != mouseXpos || inputHandler->mousey != mouseYpos) {
	mouseXpos = inputHandler->mousex;
	mouseYpos = inputHandler->mousey;
	if (inputHandler->edgeDetector) {
	    horAngle -= 0.5f * deltaTime * (width / 2.0f - mouseXpos);
	    verAngle -= 0.5f * deltaTime * (height / 2.0f - mouseYpos);
	    inputHandler->edgeDetector = false;
	}
	mouseMoved = true;
    } */
}

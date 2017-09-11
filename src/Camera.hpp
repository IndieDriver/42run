#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "env.hpp"
class Camera {
public:
	glm::vec3    pos;
	glm::vec3    dir;
	glm::vec3    right;
	glm::vec3    up;
	glm::mat4  proj;
	glm::mat4  view;
	int     width;
	int     height;
	float	mouseXpos;
	float	mouseYpos;

	Camera(glm::vec3 pos, glm::vec3 rot, int width = 0, int height = 0);
	void queryInput();
	void update();
private:
	//Vec3 target;
	bool  mouseMoved = false;
	float horAngle;
	float verAngle;
	float speed = 3.0f;
	float deltaTime;
	float lastTime;
};

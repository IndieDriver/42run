#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include "InputHandler.hpp"

class Env {
 public:
  GLFWwindow* window;
  InputHandler inputHandler;
  Env(int width, int height);

  void updateFpsCounter();
  float getDeltaTime();
  int width;
  int height;
};

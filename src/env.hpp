#pragma once
#include <array>
#include <iostream>
#include <sstream>
#include "run.hpp"

class InputHandler {
 public:
  int screenWidth;
  int screenHeight;
  std::array<bool, 1024> keys = {0};
  bool mouseDisabled = true;
  bool keybrDisabled = true;
  float mousex;
  float mousey;
};

class Env {
 public:
  GLFWwindow *window;
  InputHandler inputHandler;
  Env(int width, int height);

  void updateFpsCounter();
  float getDeltaTime();
  int width;
  int height;
};

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);

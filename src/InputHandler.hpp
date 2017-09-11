#pragma once
#include <GLFW/glfw3.h>
class InputHandler {
 public:
  int screenWidth;
  int screenHeight;
  bool keys[1024] = {0};
  bool mouseDisabled = true;
  bool keybrDisabled = true;
  float mousex;
  float mousey;
  bool edgeDetector = false;  // To smooth transition between mouse inputmode
};

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);

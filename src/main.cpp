#include "Camera.hpp"
#include "GameObject.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "env.hpp"
#include "run.hpp"

int main() {
  Env env(1280, 720);
  Shader shader("shaders/run.vert", "shaders/run.frag");

  Camera camera = Camera(glm::vec3(0.0f, 0.3f, -1.0f),
                         glm::vec3(0.0f, 0.3f, 0.0f), env.width, env.height);
  Renderer renderer(env.width, env.height);
  Scene *scene = new Scene(shader, &camera, &renderer);
  while (!glfwWindowShouldClose(env.window)) {
    env.updateFpsCounter();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();
    if (env.inputHandler.keys[GLFW_KEY_R]) {
      delete scene;
      scene = new Scene(shader, &camera, &renderer);
      env.inputHandler.keys[GLFW_KEY_R] = false;
    }
    scene->update(env.inputHandler, env.getDeltaTime());
    scene->draw();
    glfwSwapBuffers(env.window);
    GL_DUMP_ERROR("draw loop");
    if (glfwGetKey(env.window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(env.window, 1);
    }
  }
  glfwTerminate();
  delete scene;
}

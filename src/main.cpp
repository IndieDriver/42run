#include "Camera.hpp"
#include "Model.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "env.hpp"
#include "run.hpp"

std::vector<Vertex> cube_vertices = {
    {{1.0f, 1.0f, 1.0f}},    {{1.0f, -1.0f, 1.0f}},  {{1.0f, 1.0f, -1.0f}},
    {{1.0f, 1.0f, -1.0f}},   {{1.0f, -1.0f, 1.0f}},  {{1.0f, -1.0f, -1.0f}},
    {{1.0f, 1.0f, 1.0f}},    {{-1.0f, 1.0f, 1.0f}},  {{1.0f, -1.0f, 1.0f}},
    {{1.0f, -1.0f, 1.0f}},   {{-1.0f, -1.0f, 1.0f}}, {{-1.0f, 1.0f, 1.0f}},
    {{1.0f, 1.0f, 1.0f}},    {{-1.0f, 1.0f, -1.0f}}, {{-1.0f, 1.0f, 1.0f}},
    {{1.0f, 1.0f, 1.0f}},    {{1.0f, 1.0f, -1.0f}},  {{-1.0f, 1.0f, -1.0f}},
    {{-1.0f, -1.0f, -1.0f}}, {{-1.0f, -1.0f, 1.0f}}, {{-1.0f, 1.0f, 1.0f}},
    {{-1.0f, -1.0f, -1.0f}}, {{-1.0f, 1.0f, -1.0f}}, {{-1.0f, 1.0f, 1.0f}},
    {{-1.0f, -1.0f, -1.0f}}, {{-1.0f, 1.0f, -1.0f}}, {{1.0f, 1.0f, -1.0f}},
    {{-1.0f, -1.0f, -1.0f}}, {{1.0f, 1.0f, -1.0f}},  {{1.0f, -1.0f, -1.0f}},
    {{-1.0f, -1.0f, -1.0f}}, {{1.0f, -1.0f, 1.0f}},  {{-1.0f, -1.0f, 1.0f}},
    {{-1.0f, -1.0f, -1.0f}}, {{1.0f, -1.0f, -1.0f}}, {{1.0f, -1.0f, 1.0f}}};

int main() {
  Env env(1280, 720);
  Shader shader("shaders/run.vert", "shaders/run.frag");

  Camera camera(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                env.width, env.height);
  Renderer renderer;
  Scene scene(&renderer);
  scene.models.push_back(Model(shader.id, cube_vertices));
  while (!glfwWindowShouldClose(env.window)) {
    env.updateFpsCounter();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // TODO: scene.queryInput(env);
    glfwPollEvents();
    camera.queryInput(env.inputHandler.keys, env.inputHandler.mousex,
                      env.inputHandler.mousey);
    camera.update();
    renderer.view = camera.view;
    renderer.proj = camera.proj;
    scene.draw();
    glfwSwapBuffers(env.window);
    GL_DUMP_ERROR("draw loop");
    if (glfwGetKey(env.window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(env.window, 1);
    }
  }
  glfwTerminate();
}

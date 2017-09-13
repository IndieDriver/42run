#include "Camera.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "env.hpp"
#include "run.hpp"

std::vector<Vertex> cube_vertices = {
    {{0.5f, 0.5f, 0.5f}},    {{0.5f, -0.5f, 0.5f}},  {{0.5f, 0.5f, -0.5f}},
    {{0.5f, 0.5f, -0.5f}},   {{0.5f, -0.5f, 0.5f}},  {{0.5f, -0.5f, -0.5f}},
    {{0.5f, 0.5f, 0.5f}},    {{-0.5f, 0.5f, 0.5f}},  {{0.5f, -0.5f, 0.5f}},
    {{0.5f, -0.5f, 0.5f}},   {{-0.5f, -0.5f, 0.5f}}, {{-0.5f, 0.5f, 0.5f}},
    {{0.5f, 0.5f, 0.5f}},    {{-0.5f, 0.5f, -0.5f}}, {{-0.5f, 0.5f, 0.5f}},
    {{0.5f, 0.5f, 0.5f}},    {{0.5f, 0.5f, -0.5f}},  {{-0.5f, 0.5f, -0.5f}},
    {{-0.5f, -0.5f, -0.5f}}, {{-0.5f, -0.5f, 0.5f}}, {{-0.5f, 0.5f, 0.5f}},
    {{-0.5f, -0.5f, -0.5f}}, {{-0.5f, 0.5f, -0.5f}}, {{-0.5f, 0.5f, 0.5f}},
    {{-0.5f, -0.5f, -0.5f}}, {{-0.5f, 0.5f, -0.5f}}, {{0.5f, 0.5f, -0.5f}},
    {{-0.5f, -0.5f, -0.5f}}, {{0.5f, 0.5f, -0.5f}},  {{0.5f, -0.5f, -0.5f}},
    {{-0.5f, -0.5f, -0.5f}}, {{0.5f, -0.5f, 0.5f}},  {{-0.5f, -0.5f, 0.5f}},
    {{-0.5f, -0.5f, -0.5f}}, {{0.5f, -0.5f, -0.5f}}, {{0.5f, -0.5f, 0.5f}}};

int main() {
  Env env(1280, 720);
  Shader shader("shaders/run.vert", "shaders/run.frag");

  Camera camera(glm::vec3(0.0f, 0.1f, -1.0f), glm::vec3(0.0f, 0.1f, 0.0f),
                env.width, env.height);
  VAO vao_cube(cube_vertices);
  Model cubeModel = Model(shader.id, &vao_cube, glm::vec3(1.0f, 0.0f, 0.0),
                          glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f),
                          glm::vec3(1.0f, 1.0f, 1.0f));
  Renderer renderer;
  Scene scene(shader, &renderer);
  scene.cubeModel = &cubeModel;
  scene.init();
  while (!glfwWindowShouldClose(env.window)) {
    env.updateFpsCounter();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // TODO: scene.queryInput(env);
    glfwPollEvents();
    /*
    camera.queryInput(env.inputHandler.keys, env.inputHandler.mousex,
                      env.inputHandler.mousey); */
    camera.update();
    renderer.view = camera.view;
    renderer.proj = camera.proj;
    scene.update(env.inputHandler.keys);
    scene.draw();
    glfwSwapBuffers(env.window);
    // GL_DUMP_ERROR("draw loop");
    if (glfwGetKey(env.window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(env.window, 1);
    }
  }
  glfwTerminate();
}

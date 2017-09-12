#include "Camera.hpp"
#include "Model.hpp"
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
  Model cube(shader.id, cube_vertices);
  /*
     Scene scene(&camera);

     bool anim = false;
     scene.initScene(); */
  while (!glfwWindowShouldClose(env.window)) {
    env.updateFpsCounter();
    /*
    scene.queryInput(env);
    scene.draw(shader, env.particle_num);
    env.updateFpsCounter(); */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(env.window);
    glfwPollEvents();
    camera.queryInput(env.inputHandler.keys, env.inputHandler.mousex,
                      env.inputHandler.mousey);
    camera.update();
    // print_matrix(camera.view);
    /*
    renderer.proj = glm::perspective(
        45.0f, static_cast<float>(env.width) / static_cast<float>(env.height),
        1.0f, 1000.0f);
    renderer.view =
        glm::lookAt(glm::vec3(0.0f, 0.0f, -50.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f)); */
    renderer.view = camera.view;
    renderer.proj = camera.proj;
    shader.use();
    renderer.draw(cube.getRenderAttrib());
    GL_DUMP_ERROR("draw loop");
    if (glfwGetKey(env.window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(env.window, 1);
    }
  }
  glfwTerminate();
}

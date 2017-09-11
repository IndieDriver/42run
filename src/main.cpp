#include "Camera.hpp"
#include "Shader.hpp"
#include "env.hpp"

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
};

int main() {
  Env env(1280, 720);
  Shader shader("shaders/run.vert", "shaders/run.frag");
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  Camera camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                env.width, env.height);
  /*
     Scene scene(&camera);

     bool anim = false;
     scene.initScene(); */
  while (!glfwWindowShouldClose(env.window)) {
    /*
    scene.queryInput(env);
    scene.draw(shader, env.particle_num);
    env.updateFpsCounter(); */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(env.window);
    glfwPollEvents();
    camera.update();
    if (glfwGetKey(env.window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(env.window, 1);
    }
  }
  glfwTerminate();
}

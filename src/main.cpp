#include <stdio.h>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <cmath>

#include "parameters.h"
#include "fluid/fluid.h"
#include "shader/buffer.h"
#include "shader/shader.h"
#include "shader/compute.h"

char wtitle[40] = "[ FPS] OpenGL Fluid Simulation";

void windowSizeCallback(GLFWwindow* window, int width, int height);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

bool paused = true;
bool stepping = false;

int main(int, char**) {

  Phy.SMOOTHING_RADIUS = 5.f;
  Phy.KERNEL_NORMALIZATION = 4.f/(M_PI * powf(Phy.SMOOTHING_RADIUS, 4));
  Phy.GAS_CONSTANT = 30000.f;
  Phy.REST_DENSITY = 1000.f / (300.f * 500.f);
  Phy.VISCOSITY_CONSTANT = 175.f;

  fluid.setBoundaries(
    WHEIGHT/2.f - WBORDER, 
    WBORDER - WHEIGHT/2.f, 
    WBORDER - WWIDTH/2.f, 
    WWIDTH/2.f - WBORDER
  );
  fluid.initParticles(particles);
  fluid.init();
  fluid.updateGrid();

  Phy.GRAVITY.x = 0.f;
  Phy.GRAVITY.y = -100.f;
  Phy.GRAVITY.z = 0.f;

  if (!glfwInit()) {
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

  GLFWwindow* window = glfwCreateWindow(2*WWIDTH, 2*WHEIGHT, "OpenGL Fluid Simulation", NULL, NULL);
  if (window == NULL) {
    printf("Failed to create window!\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetKeyCallback(window, keyCallback);
  glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Couldn't open GLAD!\n");
    glfwTerminate();
    return -1;
  }

  Shader render_shader;
  render_shader.createProgram();
  render_shader.loadFile(Shader::VERTEX_SHADER, "assets/shaders/particle/vertex.vert");
  render_shader.loadFile(Shader::FRAGMENT_SHADER, "assets/shaders/particle/fragment.frag");
  render_shader.attach(Shader::VERTEX_SHADER);
  render_shader.attach(Shader::FRAGMENT_SHADER);
  render_shader.linkProgram();
  render_shader.deleteShader(Shader::VERTEX_SHADER);
  render_shader.deleteShader(Shader::FRAGMENT_SHADER);

  shaderbuffer_t buffers;

  glGenVertexArrays(1, &buffers.VAO);
  glGenBuffers(1, &buffers.VBO);
  // glGenBuffers(1, &EBO);
  glBindVertexArray(buffers.VAO);

  glBindBuffer(GL_ARRAY_BUFFER, buffers.VBO);
  glBufferData(GL_ARRAY_BUFFER, particles * sizeof(particle_t), fluid.particles, GL_DYNAMIC_DRAW);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * (steps-2) * particles * sizeof(float), indices, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(particle_t), (void*)(offsetof(particle_t, position)));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(particle_t), (void*)(offsetof(particle_t, velocity)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  float model_matrix[] = {
    2.f/WWIDTH, 0.f, 0.f, 0.f,
    0.f, 2.f/WHEIGHT, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
  };
  float view_matrix[] = {
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
  };
  float projection_matrix[] = {
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
  };

  glPointSize(radius);

  // glfwSetTime(0.0); // glfwGetTime returns time since simulation start.
  while (!glfwWindowShouldClose(window)) {
    double time = glfwGetTime();

    glfwPollEvents();

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Loop over all particles.
    if (!paused || stepping)
      fluid.update(TIMESTEP);
    
    if (stepping)
      stepping = false;

    glBindBuffer(GL_ARRAY_BUFFER, buffers.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, particles * sizeof(particle_t), fluid.particles);
    
    // Set model, view and projecion matrices.
    glUniformMatrix4fv(glGetUniformLocation(render_shader.program, "model"), 1, GL_FALSE, model_matrix);
    glUniformMatrix4fv(glGetUniformLocation(render_shader.program, "view"), 1, GL_FALSE, view_matrix);
    glUniformMatrix4fv(glGetUniformLocation(render_shader.program, "projection"), 1, GL_FALSE, projection_matrix);
    
    render_shader.use();
    glBindVertexArray(buffers.VAO);
    glDrawArrays(GL_POINTS, 0, particles);

    glfwSwapBuffers(window);

    while (glfwGetTime() - time < TIMESTEP); // Enforcing framerate when too fast.
    sprintf(wtitle, "[%f] OpenGL Fluid Simulation", (1/(glfwGetTime() - time)));
    glfwSetWindowTitle(window, wtitle); // Framerate now visible on window title!
  }

  glDeleteVertexArrays(1, &buffers.VAO);
    glDeleteBuffers(1, &buffers.VBO);
    glDeleteBuffers(1, &buffers.EBO);
    render_shader.deleteProgram();

    glfwTerminate();
    return 0;
}

void windowSizeCallback(GLFWwindow* window, int width, int height) {
  glfwSetWindowSize(window, width, height);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
    return;
  }
  if (key == GLFW_KEY_P && action == GLFW_PRESS) paused = !paused;
  if (key == GLFW_KEY_S && action == GLFW_PRESS) stepping = true;
    
}
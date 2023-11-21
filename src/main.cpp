#include <stdio.h>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <cmath>

#include "parameters.h"
#include "shader/shader.h"
#include "shader/compute.h"

char wtitle[40] = "[ FPS] OpenGL Fluid Simulation";

typedef struct {
    float x, y, z;
} vec3f;

struct particle {
    vec3f position;
    vec3f velocity;
};

struct particle particle_set[particles] = {
    { // Sets all struct elements to...
        .position = {.x = 0.f, .y = 0.f, .z = 0.f}, // Centered position
        .velocity = {.x = 0.f, .y = 0.f, .z = 0.f} // No velocity
    }
};

void initPositions() { // Distributes positions along line on y = 0.
    if (particles - 1)
        for (int index = 0; index < particles; index++) 
            particle_set[index].position.x = (index * .6f/(particles-1)) - .3f;
}

void windowSizeCallback(GLFWwindow* window, int width, int height);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

unsigned int VAO, VBO, EBO;

int main(int, char**) {

    initPositions();

    if (!glfwInit()) {
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(WWIDTH, WHEIGHT, "OpenGL Fluid Simulation", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create window!\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
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

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, particles * sizeof(particle), particle_set, GL_DYNAMIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * (steps-2) * particles * sizeof(float), indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(particle), (void*)(offsetof(particle, position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(particle), (void*)(offsetof(particle, velocity)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glPointSize(radius);

    // glfwSetTime(0.0); // glfwGetTime returns time since simulation start.
    while (!glfwWindowShouldClose(window)) {
        double time = glfwGetTime();

        glfwPollEvents();

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Loop over all particles.
        for (int index = 0; index < particles; index++) {
            // velocity[3*index + 1] -= .1f * TIMESTEP; // Gravity.
            particle_set[index].velocity.y -= .1f * TIMESTEP; // Gravity

            // for (int i = 3*index; i < 3*(index + 1); i++) positions[i] += velocity[i] * TIMESTEP; // Update positions
            particle_set[index].position.x += particle_set[index].velocity.x * TIMESTEP;
            particle_set[index].position.y += particle_set[index].velocity.y * TIMESTEP;
            particle_set[index].position.z += particle_set[index].velocity.z * TIMESTEP;

            // if (positions[3*index + 1] <= -.95f) { // Handle ground collision.
            //     positions[3*index + 1] = -.95f;
            //     velocity[3*index + 1] *= -1.f;
            // }
            if (particle_set[index].position.y <= -.95f) {
                particle_set[index].position.y = -.95f;
                particle_set[index].velocity.y *= -1.f;
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * particles * sizeof(float), particle_set);
        
        render_shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, particles);

        glfwSwapBuffers(window);

        while (glfwGetTime() - time < TIMESTEP); // Enforcing framerate when too fast.
        sprintf(wtitle, "[%f] OpenGL Fluid Simulation", (1/(glfwGetTime() - time)));
        glfwSetWindowTitle(window, wtitle); // Framerate now visible on window title!
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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
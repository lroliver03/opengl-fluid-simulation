#include <stdio.h>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <cmath>

#include "parameters.h"
#include "shader/shader.h"
#include "shader/compute.h"

const int steps = 16;

float vertices[] = {
    0.f, 0.f, 0.f
};
float velocity[] = {
    0.f, 0.f, 0.f
};

float circle[steps * 3];
unsigned int indices[(steps-2) * 3] = {0};

void makeCircle(float circle_center[3], float radius, unsigned int *indices, float *output) {
    float angle = 0.f;
    const float angle_step = 2*M_PI/steps;
    for (int i = 0; i < 3*steps; i += 3) {
        output[i] = radius * std::cos(angle) + circle_center[0];
        output[i+1] = radius * std::sin(angle) + circle_center[1];
        angle += angle_step;
    }
}

void windowSizeCallback(GLFWwindow* window, int width, int height);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

unsigned int VAO, VBO, EBO;

int main(int, char**) {

    for (int i = 0; i < steps - 2; i++) {
        indices[3*i] = 0;
        indices[3*i+1] = i+1;
        indices[3*i+2] = i+2;
    }

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
    render_shader.loadFile(Shader::VERTEX_SHADER, "assets/shaders/vertex.vert");
    render_shader.loadFile(Shader::FRAGMENT_SHADER, "assets/shaders/fragment.frag");
    render_shader.attach(Shader::VERTEX_SHADER);
    render_shader.attach(Shader::FRAGMENT_SHADER);
    render_shader.linkProgram();
    render_shader.deleteShader(Shader::VERTEX_SHADER);
    render_shader.deleteShader(Shader::FRAGMENT_SHADER);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    
    int width, height;
    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(circle), circle);

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        velocity[1] -= 0.0001f;

        for (int i = 0; i < 3; i++) vertices[i] += velocity[i];

        if (vertices[1] <= -.95f) {
            vertices[1] = -.95f;
            velocity[1] *= -1.f;
        }

        makeCircle(vertices, .05f, indices, circle);

        render_shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 10*steps, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    render_shader.deleteProgram();

    glfwTerminate();
    return 0;
}

void windowSizeCallback(GLFWwindow* window, int width, int height) {
    glfwSetWindowSize(window, WWIDTH, WHEIGHT); // Fixed width and height.
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, WWIDTH, WHEIGHT); // Fixed width and height.
}
#include <stdio.h>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <cmath>

#include "parameters.h"
#include "shader/shader.h"

float vertices[] = {
    0.f, 0.f, 0.f
};

float circle[12*3] = {0.f};
unsigned int indices[10*3] = {
    0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 4, 5,
    0, 5, 6,
    0, 6, 7,
    0, 7, 8,
    0, 8, 9,
    0, 9, 10,
    0, 10, 11,
};

void makeCircle(float circle_center[3], float radius, float *output) {
    float angle = 0.f;
    const float angle_step = 2*M_PI/12.f;
    for (int i = 0; i < 36; i += 3) {
        output[i] = radius * std::cos(angle) + circle_center[0];
        output[i+1] = radius * std::sin(angle) + circle_center[1];
        angle += angle_step;
    }
}

void windowSizeCallback(GLFWwindow* window, int width, int height);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

unsigned int VAO, VBO, EBO;

int main(int, char**) {

    if (!glfwInit()) {
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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

    Shader my_shader;
    my_shader.createProgram();
    my_shader.loadFile(Shader::VERTEX_SHADER, "assets/shaders/vertex.vert");
    my_shader.loadFile(Shader::FRAGMENT_SHADER, "assets/shaders/fragment.frag");
    my_shader.attach(Shader::VERTEX_SHADER);
    my_shader.attach(Shader::FRAGMENT_SHADER);
    my_shader.linkProgram();
    my_shader.deleteShader(Shader::VERTEX_SHADER);
    my_shader.deleteShader(Shader::FRAGMENT_SHADER);

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

        float time = glfwGetTime();
        float Dx = std::cos(time)/4.f;
        float Dy = std::sin(time)/4.f;
        vertices[0] = Dx;
        vertices[1] = Dy;

        makeCircle(vertices, .25f, circle);

        my_shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    my_shader.deleteProgram();

    glfwTerminate();
    return 0;
}

void windowSizeCallback(GLFWwindow* window, int width, int height) {
    glfwSetWindowSize(window, WWIDTH, WHEIGHT); // Fixed width and height.
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, WWIDTH, WHEIGHT); // Fixed width and height.
}
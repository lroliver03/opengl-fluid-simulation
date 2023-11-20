#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

int main(int, char**) {

    GLFWwindow* window;

    if (!glfwInit()) {
        return -1;
    }

    window = glfwCreateWindow(800, 600, "Window!", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Couldn't open GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glClearColor(.25f, .5f, .75f, 1.f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
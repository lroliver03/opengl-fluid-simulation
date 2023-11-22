cmake_minimum_required(VERSION 3.16)
project(opengl_fluid_simulation VERSION 0.0.0)

find_package(OpenGL REQUIRED)

add_executable(opengl_fluid_simulation
    "src/fluid/fluid.cpp"
    "src/fluid/physics.cpp"
    "src/shader/base.cpp"
    "src/shader/shader.cpp"
    "src/shader/compute.cpp"
    "src/main.cpp"
    "src/glad.c"
)

include_directories(opengl_fluid_simulation ./include)
target_link_libraries(opengl_fluid_simulation
    glfw
    OpenGL::GL
)
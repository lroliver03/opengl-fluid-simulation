#include "fluid/fluid.h"
// #include "utils/macros.h"

Fluid::Fluid(unsigned int particle_count) {
    container.top = 1.f;
    container.bottom = -1.f;
    container.left = -1.f;
    container.right = 1.f;

    particles = new particle_t[particle_count];
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            particles[i] = {
                .position = {
                    .x = SETTOINTERVAL(i, 20.f, 0.f, .3f, -.3f),
                    .y = SETTOINTERVAL(j, 20.f, 0.f, .3f, -.3f),
                    .z = 0.f//SETTOINTERVAL(i, 20.f, 0.f, .3f, -.3f)
                },
                .velocity = {
                    .x = 0.f,
                    .y = 0.f,
                    .z = 0.f
                },
                .mass = 1.f,
                .radius = .02f
            };
        }
    }

    compute.loadFile("assets/shaders/compute.comp");
    compute.createProgram();
    compute.attach();
    compute.linkProgram();
}
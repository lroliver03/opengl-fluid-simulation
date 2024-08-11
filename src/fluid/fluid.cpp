#include "fluid/fluid.h"
#include "utils/macros.h"

#include <iostream>

Fluid::Fluid() {
    // Does nothing.
}

Fluid::~Fluid() {
    delete particles;
    delete density;
    delete pressure;
}

void Fluid::initParticles(const unsigned int &particle_count) {
    count = particle_count;
    particles = new particle_t[count];
    int temp_counter = 0;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            particles[temp_counter] = {
                .position = {
                    .x = SETTOINTERVAL(j, 20.f, 0.f, WWIDTH/4.f, -WWIDTH/4.f),
                    .y = SETTOINTERVAL(i, 20.f, 0.f, -WHEIGHT/4.f, WHEIGHT/4.f),
                    .z = 0.f // SETTOINTERVAL(i, 20.f, 0.f, .3f, -.3f)
                },
                .velocity = {
                    .x = 0.f,
                    .y = 0.f,
                    .z = 0.f
                }
            };
            // std::cout << "Particle:\t" << temp_counter << "\tPosition:\t" << particles[temp_counter].position.x << "\t" << particles[temp_counter].position.y << "\t" << particles[temp_counter].position.z << std::endl;
            temp_counter++;
            if (!(temp_counter - count)) break;
        }
        if (!(temp_counter - count)) break;
    }

    density = new float[count];
    pressure = new float[count];
}

void Fluid::setBoundaries(const float &top, const float &bottom, const float &left, const float &right) {
    container = {
        .top = top,
        .bottom = bottom,
        .left = left,
        .right = right
    };
}

Fluid fluid;

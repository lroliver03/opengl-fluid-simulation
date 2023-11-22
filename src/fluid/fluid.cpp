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

void Fluid::accelerate(const unsigned int &index, const vec3f &a) {
    particles[index].velocity += a;
}

void Fluid::accelerate(const unsigned int &index, const float &ax, const float &ay, const float &az) {
    vec3f acc = {
        .x = ax,
        .y = ay,
        .z = az
    };
    this->accelerate(index, acc);
}

void Fluid::accelerateAll(const vec3f &a) {
    for (int i = 0; i < count; i++) this->accelerate(i, a);
}

void Fluid::accelerateAll(const float &ax, const float &ay, const float &az) {
    vec3f acc = {
        .x = ax,
        .y = ay,
        .z = az
    };
    this->accelerateAll(acc);
}

void Fluid::updatePositions(const float &timestep, const bool &log_values) {
    int log_count = 0;
    if (log_values)
        std::cout << "[updatePositions]: timestep = " << timestep << std::endl;
    for (int i = 0; i < count; i++) {
        particles[i].position += particles[i].velocity * timestep;
        if (particles[i].position.x < container.left) {
            particles[i].position.x = container.left;
            particles[i].velocity.x *= -1.f;
        }
        if (particles[i].position.x > container.right) {
            particles[i].position.x = container.right;
            particles[i].velocity.x *= -1.f;
        }
        if (particles[i].position.y < container.bottom) {
            particles[i].position.y = container.bottom;
            particles[i].velocity.y *= -1.f;
        }
        if (particles[i].position.y > container.top) {
            particles[i].position.y = container.top;
            particles[i].velocity.y *= -1.f;
        }
        if (log_values) {
            std::cout << "  P" << i << "\tpos: " << particles[i].position.x << "\t" << particles[i].position.y << "\t" << particles[i].position.z << "\t\t";
            if (++log_count >= 2) {
                std::cout << std::endl;
                log_count = 0;
            }
        }
    }
}

void Fluid::updateVelocities(const float &timestep, const bool &log_values) {
    int log_count = 0;
    if (log_values)
        std::cout << "[updateVelocities] timestep = " << timestep << std::endl;
    for (int i = 0; i < count; i++) {
        vec3f dv = {
            .x = 0.f,
            .y = 0.f,
            .z = 0.f
        };
        float i_force_part = pressure[i]/(density[i] * density[i]);
        for (int j = 0; j < count; j++) {
            if (j == i) continue;
            vec3f displacement = particles[i].position - particles[j].position;
            float distance = displacement.length();
            if (distance == 0.f) continue;
            vec3f direction = displacement/distance;
            dv -= direction * ((i_force_part + pressure[j]/(density[j] * density[j])) * Phy.cubedFractionSubGrad(distance, Phy.SMOOTHING_RADIUS));
        }
        dv *= timestep;
        accelerate(i, dv);
        if (log_values) {
            std::cout << "  P" << i << "\tvel: " << particles[i].velocity.x << "\t" << particles[i].velocity.y << "\t" << particles[i].velocity.z << "\t\t";
            if (++log_count >= 2) {
                std::cout << std::endl;
                log_count = 0;
            }
        }
    }
}

void Fluid::updatePressures(const bool &log_values) {
    int log_count = 0;
    if (log_values)
        std::cout << "[updatePressures]" << std::endl;
    for (int i = 0; i < count; i++) {
        pressure[i] = Phy.getPressure(density[i], Phy.IDEAL_DENSITY, Phy.PRESSURE_MULTIPLIER);
        if (log_values) {
            std::cout << "  P" << i << "\tpressure: " << pressure[i] << "\t";
            if (++log_count >= 4) {
                std::cout << std::endl;
                log_count = 0;
            }
        }
    }
}

void Fluid::updateDensities(const bool &log_values) {
    float avg = 0.f;
    int log_count = 0;
    if (log_values)
        std::cout << "[updateDensities]" << std::endl;
    for (int i = 0; i < count; i++) {
        density[i] = 0.f;
        for (int j = 0; j < count; j++) 
            density[i] += Phy.cubedFractionSub((particles[i].position - particles[j].position).length(), Phy.SMOOTHING_RADIUS);
        avg += density[i];
        if (log_values) {
            std::cout << "  P" << i << "\tdensity: " << density[i] << "\t";
            if (++log_count >= 4) {
                std::cout << std::endl;
                log_count = 0;
            }
        }
    }
    avg /= count;
    if (log_values) std::cout << "\t\tAverage density = " << avg << std::endl;
}

void Fluid::update(const float &timestep, const bool &log_values) {
    updateDensities(log_values);
    updatePressures(log_values);
    updateVelocities(timestep, log_values);
    updatePositions(timestep, log_values);
}

Fluid fluid;

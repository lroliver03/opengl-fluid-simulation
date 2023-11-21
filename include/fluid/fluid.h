#ifndef FLUID_H
#define FLUID_H

#include "../parameters.h"
#include "container.h"
#include "../shader/compute.h"

typedef struct {
    float x, y, z;
} vec3;

class Fluid {
    typedef struct {
        vec3 position;
        vec3 velocity;
        float mass;
        float radius;
    } particle_t;

    container_t container;
    particle_t *particles;
    float *vertices;

    ComputeShader compute;

    public:
        Fluid(unsigned int particle_count);
        ~Fluid();
        void sendComputeData();
};

#endif
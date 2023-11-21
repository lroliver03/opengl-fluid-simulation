#pragma once

#include "../parameters.h"
#include "container.h"
#include "../shader/shader.h"
#include "../shader/compute.h"

typedef struct {
    float x, y, z;
} vec3;

typedef struct {
    vec3 position;
    vec3 velocity;
} particle_t;

class Fluid {

    container_t container;
    particle_t *particles;
    float *vertices;

    Shader render;

    public:
        Fluid(unsigned int particle_count);
        ~Fluid();
        void attachRender(Shader &render);
        
};

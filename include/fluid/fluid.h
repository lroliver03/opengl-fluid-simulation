#pragma once

#include "../parameters.h"
#include "container.h"
#include "physics.h"
// #include "../shader/buffer.h"
// #include "../shader/shader.h"
// #include "../shader/compute.h"

class Fluid {
    public:
        unsigned int count;
        container_t container;
        particle_t *particles;
        float *density;
        float *pressure;

        Fluid();
        ~Fluid();

        void initParticles(const unsigned int &particle_count);
        void setBoundaries(const float &top, const float &bottom, const float &left, const float &right);
        
};

extern Fluid fluid;

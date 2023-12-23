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

        void accelerate(const unsigned int &index, const vec3f &a);
        void accelerate(const unsigned int &index, const float &ax, const float &ay, const float &az);
        void accelerateAll(const vec3f &a);
        void accelerateAll(const float &ax, const float &ay, const float &az);

        void updatePositions(const float &timestep, const bool &log_values = false);
        void updateVelocities(const float &timestep, const bool &log_values = false);
        void updatePressures(const bool &log_values = false);
        void updateDensities(const bool &log_values = false);

        void update(const float &timestep, const bool &log_values = false);
        
};

extern Fluid fluid;

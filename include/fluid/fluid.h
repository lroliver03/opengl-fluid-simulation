#pragma once

#include "../parameters.h"
#include "container.h"
#include "physics.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
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
    
    struct grid_t {
      std::vector<std::vector<unsigned int>> vec;
      int width, height;
      inline unsigned int getIndex(const unsigned &x, const unsigned &y);
    } grid;

    Fluid();
    ~Fluid();

    void init();
    void initParticles(const unsigned int &particle_count);
    void setBoundaries(const float &top, const float &bottom, const float &left, const float &right);

    inline void accelerate(const unsigned &particle, const vec3f &dv);

    inline void updateAllDensities();
    inline void updateAllPressures();
    inline void updateAllVelocities(const float &timestep);
    inline void handleBorderCollisions(const unsigned int &particle);
    inline void updateAllPositions(const float &timestep);

    void updateGrid();

    inline void updateDensity(const unsigned &particle);
    // inline void updatePressure(const unsigned &particle);
    inline void updateVelocity(const unsigned &particle, const float &timestep);
    inline void updatePosition(const unsigned &particle, const float &timestep);

    void update(const float &timestep);
};

extern Fluid fluid;

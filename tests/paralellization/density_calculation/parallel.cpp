#include <iostream>
#include <fstream>
#include <cmath>
#include <omp.h>

#define LOOPS 1000
#define NUM_THREADS 8

#define PARTICLES 400
#define VECLEN(v) (std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z))

const float SMOOTHING_RADIUS = 40.f;
float smoothingKernel(const float &distance, const float &radius) {
    if (distance >= radius) return 0.f;
    float value = 1 - distance/radius;
    return value * value * value;
}

// Copied straight from physics.h
typedef struct vec3f {
    float x, y, z;

    vec3f operator-(const vec3f &v) const {
        vec3f result = {
            .x = x - v.x,
            .y = y - v.y,
            .z = z - v.z
        };
        return result;
    }

} vec3f;

vec3f positions[PARTICLES] = {{.x = 0.f, .y = 0.f, .z = 0.f}};
float densities[PARTICLES] = {0.f};

int main() {

    omp_set_num_threads(NUM_THREADS);

    std::ifstream source;
    source.open("positions.txt", std::ios_base::in);
    for (int i = 0; i < PARTICLES; i++) source >> positions[i].x >> positions[i].y >> positions[i].z;
    source.close();

    for (int iter = 0; iter < LOOPS; iter++) {
        for (int i = 0; i < PARTICLES; i++) {
            densities[i] = 0.f;
            vec3f current_pos = positions[i];
            #pragma omp parallel for reduction (+:densities[i])
            for (int j = 0; j < PARTICLES; j++) {
                densities[i] += smoothingKernel(VECLEN((current_pos - positions[j])), SMOOTHING_RADIUS);
            }
        }
    }

    std::cout << "Density values:" << std::endl;
    for (int i = 0; i < PARTICLES; i++) std::cout << i << " - " << densities[i] << std::endl;

    return 0;
}
#pragma once

#include <random>
#include <cmath>
#include <iostream>

typedef struct vec3f {
  float x, y, z;

  vec3f operator+(const vec3f &v) const;
  vec3f operator-(const vec3f &v) const;
  vec3f operator*(const float &v) const;
  vec3f operator/(const float &v) const;
  vec3f& operator=(const vec3f &v);
  vec3f& operator+=(const vec3f &v);
  vec3f& operator-=(const vec3f &v);
  vec3f& operator*=(const float &v);
  vec3f& operator/=(const float &v);
  vec3f operator-() const;

  float length() const;
  inline vec3f dir() const;
} vec3f;

inline float dot(const vec3f &v1, const vec3f &v2);
inline vec3f cross(const vec3f &v1, const vec3f &v2);

std::ostream &operator<<(std::ostream &out, const vec3f &v);

struct gridpos_t {
  int x, y;
};

typedef struct particle_t {
  vec3f position;
  vec3f velocity;
  struct gridpos_t grid_position;
} particle_t;

class Physics {
  public:

    float SMOOTHING_RADIUS;
    float KERNEL_NORMALIZATION;
    float GAS_CONSTANT;
    float REST_DENSITY;
    float VISCOSITY_CONSTANT;
    float SURFACE_TENSION_CONSTANT;
    float SURFACE_TENSION_THRESHOLD;

    vec3f GRAVITY;

    // Kernel functions used for generic property calculations.
    /* spikyKernel takes vector and computes kernel from its length. */
    float spikyKernel(const vec3f &r) const;
    /* gradSpikyKernel takes vector and computes kernel gradient vector from it. */
    vec3f gradSpikyKernel(const vec3f &r) const;
    /* laplSpikyKernel takes vector and computes kernel Laplacian from its length. */
    float laplSpikyKernel(const vec3f &r) const;

    float getPressure(const float &density);

    vec3f getRandomDirection();
};

extern const vec3f RIGHT, LEFT, UP, DOWN, FRONT, BACK, ZEROVEC; // Unit base vectors.
extern Physics Phy;
#pragma once

#include <random>
#include <cmath>

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
    vec3f dir() const;
} vec3f;

float dot(const vec3f &v1, const vec3f &v2);
vec3f cross(const vec3f &v1, const vec3f &v2);

typedef struct particle_t {
    vec3f position;
    vec3f next_position;
    vec3f velocity;
} particle_t;

class Physics {
    public:

        vec3f getRandomDirection();
};

extern const vec3f RIGHT, LEFT, UP, DOWN, FRONT, BACK; // Unit base vectors.
extern Physics Phy;
#include "fluid/physics.h"
#include <cmath>

vec3f vec3f::operator+(const vec3f &v) const {
    vec3f result = {
        .x = x + v.x,
        .y = y + v.y,
        .z = z + v.z
    };
    return result;
}

vec3f vec3f::operator-(const vec3f &v) const {
    vec3f result = {
        .x = x - v.x,
        .y = y - v.y,
        .z = z - v.z
    };
    return result;
}

vec3f vec3f::operator*(const float &v) const {
    vec3f result = {
        .x = x * v,
        .y = y * v,
        .z = z * v
    };
    return result;
}

vec3f vec3f::operator/(const float &v) const {
    vec3f result = {
        .x = x / v,
        .y = y / v,
        .z = z / v
    };
    return result;
}

vec3f& vec3f::operator=(const vec3f &v) {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

vec3f& vec3f::operator+=(const vec3f &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

vec3f& vec3f::operator-=(const vec3f &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

vec3f& vec3f::operator*=(const float &v) {
    x *= v;
    y *= v;
    z *= v;
    return *this;
}

vec3f& vec3f::operator/=(const float &v) {
    x /= v;
    y /= v;
    z /= v;
    return *this;
}

vec3f vec3f::operator-() const {
    vec3f result = {
        .x = -x,
        .y = -y,
        .z = -z
    };
    return result;
}

float vec3f::length() const {
    return std::sqrt(x*x + y*y + z*z);
}

vec3f vec3f::dir() const {
    return (*this)/this->length();
}

float Physics::squaredFractionSub(const float &length, const float &radius) const {
    if (length >= radius) return 0.f;
    float value = 1 - length/radius;
    return value * value;
}

float Physics::cubedFractionSub(const float &length, const float &radius) const {
    if (length >= radius) return 0.f;
    float value = 1 - length/radius;
    return value * value * value;
}

float Physics::squaredFractionSubGrad(const float &length, const float &radius) const {
    if (length >= radius) return 0.f;
    float value = 1 - length/radius;
    return -2.f * value/radius;
}

float Physics::cubedFractionSubGrad(const float &length, const float &radius) const {
    if (length >= radius) return 0.f;
    float value = 1 - length/radius;
    return -3.f * value * value/radius;
}

float Physics::getPressure(const float &density, const float &ideal_density, const float &multiplier) const {
    return multiplier * (density - ideal_density);
}

const vec3f RIGHT = {.x =  1.f, .y =  0.f, .z =  0.f};
const vec3f LEFT =  {.x = -1.f, .y =  0.f, .z =  0.f};
const vec3f UP =    {.x =  0.f, .y =  1.f, .z =  0.f};
const vec3f DOWN =  {.x =  0.f, .y = -1.f, .z =  0.f};
const vec3f FRONT = {.x =  0.f, .y =  0.f, .z =  1.f};
const vec3f BACK =  {.x =  0.f, .y =  0.f, .z = -1.f};

Physics Phy;

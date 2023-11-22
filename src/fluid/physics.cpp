#include "fluid/physics.h"

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

float dot(const vec3f &v1, const vec3f &v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

vec3f cross(const vec3f &v1, const vec3f &v2) {
    vec3f result = {
        .x = v1.y*v2.z - v2.y*v1.z,
        .y = v1.z*v2.x - v2.x*v1.z,
        .z = v1.x*v2.y - v2.x*v1.y
    };
    return result;
}

float Physics::fractionSub(const float &length, const float &radius) const {
    if (length >= radius) return 0.f;
    return 1 - length/radius;
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

vec3f Physics::getRandomDirection() {
    static std::random_device rd;
    static std::mt19937 rng(rd());
    static std::uniform_real_distribution<> real_dist(0.f, 1.f);

    float phi = 360.f * real_dist(rng);
    // float theta = 180.f * RANDOMF(); // Will be implemented in 3D!
    vec3f random_dir = {
        .x = std::cos(phi),
        .y = std::sin(phi),
        .z = 0.f
    };
    return random_dir;
}

const vec3f RIGHT = {.x =  1.f, .y =  0.f, .z =  0.f};
const vec3f LEFT =  {.x = -1.f, .y =  0.f, .z =  0.f};
const vec3f UP =    {.x =  0.f, .y =  1.f, .z =  0.f};
const vec3f DOWN =  {.x =  0.f, .y = -1.f, .z =  0.f};
const vec3f FRONT = {.x =  0.f, .y =  0.f, .z =  1.f};
const vec3f BACK =  {.x =  0.f, .y =  0.f, .z = -1.f};

Physics Phy;

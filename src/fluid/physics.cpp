#include "fluid/physics.h"

const vec3f RIGHT   = {.x =  1.f, .y =  0.f, .z =  0.f};
const vec3f LEFT    = {.x = -1.f, .y =  0.f, .z =  0.f};
const vec3f UP      = {.x =  0.f, .y =  1.f, .z =  0.f};
const vec3f DOWN    = {.x =  0.f, .y = -1.f, .z =  0.f};
const vec3f FRONT   = {.x =  0.f, .y =  0.f, .z =  1.f};
const vec3f BACK    = {.x =  0.f, .y =  0.f, .z = -1.f};
const vec3f ZEROVEC = {.x =  0.f, .y =  0.f, .z =  0.f};

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

inline float vec3f::length() const {
  return std::sqrt(x*x + y*y + z*z);
}

inline vec3f vec3f::dir() const {
  float length = this->length();
  if (length <= 0.f) 
    return ZEROVEC;
  return (*this)/this->length();
}

inline float dot(const vec3f &v1, const vec3f &v2) {
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

inline vec3f cross(const vec3f &v1, const vec3f &v2) {
  vec3f result = {
    .x = v1.y*v2.z - v2.y*v1.z,
    .y = v1.z*v2.x - v2.x*v1.z,
    .z = v1.x*v2.y - v2.x*v1.y
  };
  return result;
}

inline std::ostream &operator<<(std::ostream &out, const vec3f &v) {
  return out << "[" << v.x << ", " << v.y << ", " << v.z << "]";
}

float Physics::spikyKernel(const vec3f &r) const {
  float length = r.length();
  if (length >= this->SMOOTHING_RADIUS)
    return 0.f;
  float distance = this->SMOOTHING_RADIUS - length;
  float distance_cubed = distance * distance * distance;
  // Returns 2*pi*(h-r)^3/15. As per the paper.
  return this->KERNEL_NORMALIZATION * distance_cubed;
}

vec3f Physics::gradSpikyKernel(const vec3f &r) const {
  float length = r.length();
  if (length >= this->SMOOTHING_RADIUS)
    return ZEROVEC;
  float distance = this->SMOOTHING_RADIUS - length;
  float distance_squared = distance * distance;
  vec3f direction = r.dir();
  return direction * (-3.f * this->KERNEL_NORMALIZATION * distance_squared);
}

float Physics::laplSpikyKernel(const vec3f &r) const {
  float length = r.length();
  if (length >= this->SMOOTHING_RADIUS)
    return 0.f;
  float distance = this->SMOOTHING_RADIUS - length;
  return 6.f * this->KERNEL_NORMALIZATION * distance;
}

float Physics::getPressure(const float &density) {
  // Computing the difference from rest density makes the simulation more stable.
  return this->GAS_CONSTANT * (density - this->REST_DENSITY);
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

Physics Phy;

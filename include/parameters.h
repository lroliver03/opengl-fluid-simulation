#pragma once

#include "utils/macros.h"

#define WWIDTH 800
#define WHEIGHT 800
#define WBORDER 50

const unsigned int particles = 10;
const float radius = 10.f;
const float normalized_radius = radius/WWIDTH;
const float FRAMERATE = 60.f;
const float TIMESTEP = 1/FRAMERATE;

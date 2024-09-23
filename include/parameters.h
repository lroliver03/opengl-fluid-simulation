#pragma once

#include "utils/macros.h"

#define WWIDTH 600
#define WHEIGHT 400
#define WBORDER 50

#define EPSILON .1f

const unsigned int particles = 3000;
const float radius = 3.f;
const float FRAMERATE = 60.f;
const float TIMESCALE = 1.f;
const float TIMESTEP = TIMESCALE/FRAMERATE;

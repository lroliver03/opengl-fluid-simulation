#pragma once

#include "../parameters.h"
#include "../utils/macros.h"

#define CWIDTH_PX (WWIDTH - 2*WBORDER)
#define CHEIGHT_PX (WHEIGHT - 2*WBORDER)

struct container_t {
  float top;
  float bottom;
  float left;
  float right;
};

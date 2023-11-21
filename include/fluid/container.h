#pragma once

#include "../parameters.h"
#include "../utils/macros.h"

#define CWIDTH_PX (WWIDTH - 2*WBORDER)
#define CHEIGHT_PX (WHEIGHT - 2*WBORDER)

#define CWIDTH_GL (NORMALIZE(CWIDTH_PX, 0.f, WWIDTH))
#define CHEIGHT_GL (NORMALIZE(CHEIGHT_PX, 0.f, WHEIGHT))

struct container_t {
    float top;
    float bottom;
    float left;
    float right;
};

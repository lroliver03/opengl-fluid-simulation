#pragma once

#define SETTOINTERVAL(x, old_top, old_bottom, new_top, new_bottom) ((x - old_bottom) * (new_top - new_bottom)/old_top + new_bottom)
#define NORMALIZE(x, top, bottom) (SETTOINTERVAL(x, top, bottom, 1.f, -1.f))
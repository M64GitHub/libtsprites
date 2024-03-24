// tseffects.hpp - libtsprites, 2023-24 M64

#ifndef TSL_EFFECTS_H
#define TSL_EFFECTS_H

#include "tsrender.hpp"

void dim(render_surface *r, int step, int maxsteps);
void fadeIn(render_surface *r, int step, int maxsteps);
void fadeOut(render_surface *r, int step, int maxsteps);

#endif


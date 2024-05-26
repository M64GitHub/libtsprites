// tseffects.hpp - libtsprites, 2023-24 M64

#ifndef TSL_EFFECTS_H
#define TSL_EFFECTS_H

#include "tsrendersurface.hpp"

void dim_render_surface(render_surface *r, int step, int maxsteps);
void dim_render_surface_in_out(render_surface *r_in, int step, int maxsteps,
                        render_surface *r_out);

#endif


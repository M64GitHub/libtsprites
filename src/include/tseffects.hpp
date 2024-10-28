// tseffects.hpp - libtsprites, 2023-24 M64

#ifndef TSL_EFFECTS_H
#define TSL_EFFECTS_H

#include "tsrendersurface.hpp"

void dim_render_surface(RenderSurface_t *r, int step, int maxsteps);
void dim_render_surface_in_out(RenderSurface_t *r_in, int step, int maxsteps,
                               RenderSurface_t *r_out);

#endif

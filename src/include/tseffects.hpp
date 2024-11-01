// tseffects.hpp - libtsprites, 2023-24 M64

#ifndef TSL_EFFECTS_H
#define TSL_EFFECTS_H

#include "tscolors.hpp"
#include "tsrendersurface.hpp"
#include "tstypes.h"

void tsfx_dim_direct(RenderSurface_t *r, int step, int maxsteps);
void tsfx_dim(RenderSurface_t *r_in, int step, int maxsteps,
              RenderSurface_t *r_out);

void tsfx_color_fill(RenderSurface_t *r_in, RGBColor_t *color_in,
                     RGBColor_t *color_out, RenderSurface_t *r_out);

void tsfx_char_fill(RenderSurface_t *r_in, RGBColor_t *color_in,
                    RGBColor_t *color_out, unsigned char c,
                    RenderSurface_t *r_out);

void tsfx_outline_rotor_tick(RenderSurface_t *r, int start_x, int start_y,
                             TS_DIRECTION dir);

#endif

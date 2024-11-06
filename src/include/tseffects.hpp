// tseffects.hpp - libtsprites, 2023-24 M64

#ifndef TSL_EFFECTS_H
#define TSL_EFFECTS_H

#include "tscolors.hpp"
#include "tsrendersurface.hpp"
#include "tstypes.h"

#define PLASMA_REPLACE 3

typedef struct TSFXPlasmaCTX_s {
  int time_offset;
  int width;
  int heigth;
  RGBPalette_t palette;
  float clamp_range;
  RGBColor_t replace_color;
  RenderSurface_t *rs;
} TSFXPlasmaCTX_t;

void tsfx_dim_direct(RenderSurface_t *r, int step, int maxsteps);
void tsfx_dim(RenderSurface_t *r_in, int step, int maxsteps,
              RenderSurface_t *r_out);

void tsfx_dim_bgcolor(RenderSurface_t *r_in, int step, int maxsteps,
                      RenderSurface_t *r_out, RGBColor_t c);

void tsfx_color_fill(RenderSurface_t *r_in, RGBColor_t *color_in,
                     RGBColor_t *color_out, RenderSurface_t *r_out);

void tsfx_char_fill(RenderSurface_t *r_in, RGBColor_t *color_in,
                    RGBColor_t *color_out, unsigned char c,
                    RenderSurface_t *r_out);

void tsfx_outline_rotor_tick(RenderSurface_t *r, int start_x, int start_y,
                             TS_DIRECTION dir);

void tsfx_plasma_init_palette(TSFXPlasmaCTX_t *pctx);
void tsfx_plasma_generate_plasma(TSFXPlasmaCTX_t *pctx, RenderSurface_t *ro);

void tsfx_plasma_init(TSFXPlasmaCTX_t *pctx, int width, int height,
                      int num_colors, float clamp_range,
                      RGBColor_t replace_color, RenderSurface_t *rs);

#endif

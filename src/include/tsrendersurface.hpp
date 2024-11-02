// tsrendersurface.h - libtsprites, 2023-24 M64

#ifndef TSL_RENDER_SURFACE_H
#define TSL_RENDER_SURFACE_H

#include "tstypes.h"
#include "tscolors.hpp"

typedef struct RenderSurface_s {
  int w; // measured in blocks, 1 ASCI char has width 1
  int h; // measured in blocks, 1 ASCII char has height 2, 1 block height

  int x;
  int y;

  int z; // layer - for sorting by rendering engine

  RGBColor_t *colormap;
  unsigned char *shadowmap;
  unsigned char *charmap;
} RenderSurface_t;

void render_surface_init(RenderSurface_t *s, int w, int h, RGBColor_t r);
void render_surface_free(RenderSurface_t *s);

void render_surface_clear_colored(RenderSurface_t *s, RGBColor_t c);
void render_surface_clear_transparent(RenderSurface_t *s);

int render_surface_copy(RenderSurface_t *in, RenderSurface_t *out);

void render_surface_get_color(RenderSurface_t *r, Pixel2D_t *p);
bool render_surface_has_color(RenderSurface_t *r, Pixel2D_t *p);
bool render_surface_has_color(RenderSurface_t *r, int x, int y);
bool render_surface_has_color_thr(RenderSurface_t *r, Pixel2D_t *p, int thr);
bool render_surface_has_color_thr(RenderSurface_t *r, int x, int y, int thr);
void render_surface_set_color(RenderSurface_t *r, Pixel2D_t *p);

#endif

// tsrendersurface.h - libtsprites, 2023-24 M64

#ifndef TSL_RENDER_SURFACE_H
#define TSL_RENDER_SURFACE_H

#include "tscolors.hpp"

typedef struct RenderSurface_s {
  int w; // measured in blocks, 1 ASCI char has width 1
  int h; // measured in blocks, 1 ASCII char has height 2, 1 block height

  int x;
  int y;

  int z; // layer - for sorting by rendering engine

  RGBColor_t *colormap;
  unsigned char *shadowmap;
  int is_updated = 0; // "need render" f sprites:
                      // anis set this, effects, etc.
                      // set_frameidx, etc
                      // if 0, sprite doesn't re-render itself
                      // just waisting time
  unsigned char *charmap;
} RenderSurface_t;

void render_surface_init(RenderSurface_t *s, int w, int h, RGBColor_t r);

void render_surface_clear_colored(RenderSurface_t *s, RGBColor_t c);
void render_surface_clear_transparent(RenderSurface_t *s);

int copy_surface_contents(RenderSurface_t *in, RenderSurface_t *out);

#endif

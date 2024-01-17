#ifndef TSL_RENDER_SURFACE_H
#define TSL_RENDER_SURFACE_H

#include "tscolors.hpp"

typedef struct s_render_surface {
  int w; // measured in blocks, 1 ASCI char has width 1
  int h; // measured in blocks, 1 ASCII char has height 2, 1 block height 

  int x;
  int y;

  int z; // layer - for sorting by rendering engine

  rgb_color *colormap;
  unsigned char *shadowmap;
  int is_updated = 0; // "need render" f sprites:
                      // anis set this, effects, etc.
                      // set_frameidx, etc
                      // if 0, sprite doesn't re-render itself
                      // just waisting time
} render_surface;

#endif

// tsrender.cpp - libtsprites 2023-24, M64

#include "include/tsrender.hpp"
#include <stdio.h>

// RenderEngine1:
// renders all input surfaces onto the output surface (and returns it)
// Basic operation:
//  - sort all surfaces by z. Higher z means more on top. z is rendered over
//    z-1. Highest z shall be 1st in sorted array. same values of z:
//    to be defined (TODO!!).
//  - go through all surfaces in sorted order, and:
//    - copy intransparent pixels to surface_out, where shadowmap of '
//      surface_out == 0 (or -1?)
//      - write the nr of the (input) surface to shadow_map of this pixel on
//        surface_out
render_surface *TSRenderEngineTopDown::Render(render_surface **surfaces_in,
                                              int n,
                                              render_surface *surface_out) {
  if (!n)
    return 0;

  printf("RENDER: %d surfaces\n", n);
  fflush(stdout);

  // render_surface **surfaces_sortuuuuued = new render_surface *[n];
  int *tmp = new int[n];
  for (int i = 0; i < n; i++)
    tmp[i] = 0;

  // -- sort / prepare
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      ;
    }
  }

  // -- render

  int idx_in = 0;
  int idx_out = 0;
  for (int i = 0; i < n; i++) {
    for (int Y = 0; Y < surfaces_in[i]->h; Y++) {
      for (int X = 0; X < surfaces_in[i]->w; X++) {

        idx_in = surfaces_in[i]->w * Y + X;
        idx_out =
            surface_out->w * (Y + surfaces_in[i]->y) + X + surfaces_in[i]->x;

        if (surfaces_in[i]->shadowmap[idx_in] &&
            !surface_out->shadowmap[idx_out]) {
          if (!((surfaces_in[i]->x + X) >= surface_out->w) &&   // clip x right
              !((surfaces_in[i]->x + X) < 0)) {                 // clip x left
            if (!((surfaces_in[i]->y + Y) >= surface_out->h) && // clip x right
                !((surfaces_in[i]->y + Y) < 0)) {               // clip x left
              surface_out->colormap[idx_out] = surfaces_in[i]->colormap[idx_in];
              surface_out->shadowmap[idx_out] = 1;
            } // clipping Y
          }   // clipping X
        }     // if
      }       // X
    }         // Y
  }
  // -- cleanup

  delete[] tmp;
  // delete[] surfaces_sorted;

  return surface_out;
}

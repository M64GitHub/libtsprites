// tsrender.cpp - libtsprites 2023-24, M64

#include "include/tsrender.hpp"

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
  render_surface **surfaces_sorted = new render_surface *[n];
  int *tmp = new int[n];
  
  for(int i=0; i<n; i++) tmp[i] = 0;

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) {
      ;
    }

  delete[] tmp;
  delete[] surfaces_sorted;

  return surface_out;
}

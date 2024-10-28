
// tsanimations.cpp - libtsprites, 2023-24 M64

#include "include/tsrendersurface.hpp"

void init_surface(render_surface *s, int w, int h, rgb_color c) {
  if (!s)
    return;
  s->w = w;
  s->h = h;
  s->colormap = new rgb_color[s->w * s->h];
  s->shadowmap = new unsigned char[s->w * s->h];
  s->charmap = new unsigned char[s->w * s->h];
  clear_surface_bgcolor(s, c);
}

void clear_surface_bgcolor(render_surface *s, rgb_color c) {
  if (!s || !s->colormap || !s->shadowmap)
    return;
  for (int i = 0; i < s->w * s->h; i++) {
    s->colormap[i] = c;
    s->shadowmap[i] = 2;
    s->charmap[i] = 0;
  }
}

void clear_surface_transparent(render_surface *s) {
  if (!s || !s->colormap || !s->shadowmap)
    return;
  rgb_color c = { 0x00, 0x00, 0x00};
  for (int i = 0; i < s->w * s->h; i++) {
    s->colormap[i] = c;
    s->shadowmap[i] = 0;
    s->charmap[i] = 0;
  }
}

int copy_surface_contents(render_surface *in, render_surface *out) {
  if(!in) return 1;
  if(!out) return 2;
  if(!in->colormap || !in->shadowmap || !in->charmap) return 3;
  if(!in->w || !in->h) return 4;

  for (int i = 0; i < (in->w * in->h); i++) {
    out->colormap[i] = in->colormap[i];
    out->shadowmap[i] = in->shadowmap[i];
    out->charmap[i] = in->charmap[i];
  }

  out->w = in->w;
  out->h = in->h;
  out->x = in->x;
  out->y = in->y;
  out->z = in->z;

  return 0;
}

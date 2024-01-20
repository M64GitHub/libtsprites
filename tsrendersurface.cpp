#include "include/tsrendersurface.hpp"

void init_surface(render_surface *s, int w, int h, rgb_color c) {
  if (!s)
    return;
  s->w = w;
  s->h = h;
  s->colormap = new rgb_color[s->w * s->h];
  s->shadowmap = new unsigned char[s->w * s->h];
  clear_surface_bgcolor(s, c);
}

void clear_surface_bgcolor(render_surface *s, rgb_color c) {
  if (!s || !s->colormap || !s->shadowmap)
    return;
  for (int i = 0; i < s->w * s->h; i++) {
    s->colormap[i] = c;
    s->shadowmap[i] = 2;
  }
}

void clear_surface_transparent(render_surface *s) {
  if (!s || !s->colormap || !s->shadowmap)
    return;
  rgb_color c = { 0x00, 0x00, 0x00};
  for (int i = 0; i < s->w * s->h; i++) {
    s->colormap[i] = c;
    s->shadowmap[i] = 0;
  }
}

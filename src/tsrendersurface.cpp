
// tsanimations.cpp - libtsprites, 2023-24 M64

#include "include/tsrendersurface.hpp"
#include "include/tscolors.hpp"

void render_surface_init(RenderSurface_t *s, int w, int h, RGBColor_t c) {
  if (!s)
    return;
  s->w = w;
  s->h = h;
  s->colormap = new RGBColor_t[s->w * s->h];
  s->shadowmap = new unsigned char[s->w * s->h];
  s->charmap = new unsigned char[s->w * s->h];
  render_surface_clear_colored(s, c);
}

void render_surface_clear_colored(RenderSurface_t *s, RGBColor_t c) {
  if (!s || !s->colormap || !s->shadowmap)
    return;
  for (int i = 0; i < s->w * s->h; i++) {
    s->colormap[i] = c;
    s->shadowmap[i] = 2;
    s->charmap[i] = 0;
  }
}

void render_surface_clear_transparent(RenderSurface_t *s) {
  if (!s || !s->colormap || !s->shadowmap)
    return;
  RGBColor_t c = {0x00, 0x00, 0x00};
  for (int i = 0; i < s->w * s->h; i++) {
    s->colormap[i] = c;
    s->shadowmap[i] = 0;
    s->charmap[i] = 0;
  }
}

int render_surface_copy(RenderSurface_t *in, RenderSurface_t *out) {
  if (!in)
    return 1;
  if (!out)
    return 2;
  if (!in->colormap || !in->shadowmap || !in->charmap)
    return 3;
  if (!out->colormap || !out->shadowmap || !out->charmap)
    return 4;
  if (!in->w || !in->h)
    return 4;

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

void render_surface_get_color(RenderSurface_t *r, Pixel2D_t *p) {
  if (!r || !p)
    return;
  p->c = {-1, -1, -1};
  if ((p->x < 0) || (p->x >= r->w))
    return;
  if ((p->y < 0) || (p->y >= r->h))
    return;

  p->c = r->colormap[r->w * p->y + p->x];
}

void render_surface_set_color(RenderSurface_t *r, Pixel2D_t *p) {
  if (!r || !p)
    return;
  if ((p->x < 0) || (p->x >= r->w))
    return;
  if ((p->y < 0) || (p->y >= r->h))
    return;

  r->colormap[r->w * p->y + p->x] = p->c;
}

bool render_surface_has_color(RenderSurface_t *r, int x, int y) {
  if (!r)
    return false;
  if ((x < 0) || (x >= r->w))
    return false;
  if ((y < 0) || (y >= r->h))
    return false;

  if (r->shadowmap[r->w * y + x])
    return true;

  return false;
}

bool render_surface_has_color(RenderSurface_t *r, Pixel2D_t *p) {
  if (!r || !p)
    return false;
  if ((p->x < 0) || (p->x >= r->w))
    return false;
  if ((p->y < 0) || (p->y >= r->h))
    return false;

  if (r->shadowmap[r->w * p->y + p->x])
    return true;

  return false;
}

bool render_surface_has_color_thr(RenderSurface_t *r, int x, int y, int thr) {
  if (!r)
    return false;
  if ((x < 0) || (x >= r->w))
    return false;
  if ((y < 0) || (y >= r->h))
    return false;

  if (r->shadowmap[r->w * y + x]) {
    RGBColor_t c = r->colormap[r->w * y + x];
    if (c.r > thr)
      return true;
    if (c.g > thr)
      return true;
    if (c.b > thr)
      return true;
  }

  return false;
}

bool render_surface_has_color_thr(RenderSurface_t *r, Pixel2D_t *p, int thr) {
  if (!r || !p)
    return false;
  if ((p->x < 0) || (p->x >= r->w))
    return false;
  if ((p->y < 0) || (p->y >= r->h))
    return false;

  if (r->shadowmap[r->w * p->y + p->x]) {
    RGBColor_t c = r->colormap[r->w * p->y + p->x];
    if (c.r > thr)
      return true;
    if (c.g > thr)
      return true;
    if (c.b > thr)
      return true;
  }

  return false;
}

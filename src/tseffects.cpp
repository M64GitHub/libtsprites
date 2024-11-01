// tseffects.cpp - libtsprites, 2023-24 M64

#include "include/tseffects.hpp"
#include "include/tscolors.hpp"
#include "include/tsrendersurface.hpp"
#include "include/tstypes.h"

void tsfx_dim_direct(RenderSurface_t *r, int step, int maxsteps) {
  double dr, dg, db;
  for (int Y = 0; Y < r->h; Y++) {
    for (int X = 0; X < r->w; X++) {
      dr = r->colormap[Y * r->w + X].r;
      dg = r->colormap[Y * r->w + X].g;
      db = r->colormap[Y * r->w + X].b;

      dr = dr * (double)step;
      dr = dr / (double)maxsteps;
      dg = dg * (double)step;
      dg = dg / (double)maxsteps;
      db = db * (double)step;
      db = db / (double)maxsteps;

      if (dr >= 255)
        dr = 255;
      if (dg >= 255)
        dg = 255;
      if (db >= 255)
        db = 255;

      r->colormap[Y * r->w + X].r = dr;
      r->colormap[Y * r->w + X].g = dg;
      r->colormap[Y * r->w + X].b = db;
    }
  }
}

void tsfx_dim(RenderSurface_t *r_in, int step, int maxsteps,
              RenderSurface_t *r_out) {
  double dr, dg, db;
  for (int Y = 0; Y < r_in->h; Y++) {
    for (int X = 0; X < r_in->w; X++) {
      dr = r_in->colormap[Y * r_in->w + X].r;
      dg = r_in->colormap[Y * r_in->w + X].g;
      db = r_in->colormap[Y * r_in->w + X].b;

      dr = dr * (double)step;
      dr = dr / (double)maxsteps;
      dg = dg * (double)step;
      dg = dg / (double)maxsteps;
      db = db * (double)step;
      db = db / (double)maxsteps;

      if (dr >= 255)
        dr = 255;
      if (dg >= 255)
        dg = 255;
      if (db >= 255)
        db = 255;

      r_out->colormap[Y * r_out->w + X].r = dr;
      r_out->colormap[Y * r_out->w + X].g = dg;
      r_out->colormap[Y * r_out->w + X].b = db;
    }
  }
}

void tsfx_color_fill(RenderSurface_t *r_in, RGBColor_t *color_in,
                     RGBColor_t *color_out, RenderSurface_t *r_out) {
  for (int Y = 0; Y < r_in->h; Y++) {
    for (int X = 0; X < r_in->w; X++) {
      if (rgb_equal(color_in, &r_in->colormap[Y * r_in->w + X])) {
        r_out->colormap[Y * r_out->w + X] = *color_out;
      }
    }
  }
}

void tsfx_char_fill(RenderSurface_t *r_in, RGBColor_t *color_in,
                    RGBColor_t *color_out, unsigned char c,
                    RenderSurface_t *r_out) {
  for (int Y = 0; Y < r_in->h; Y++) {
    for (int X = 0; X < r_in->w; X++) {
      if (rgb_equal(color_in, color_out)) {
        r_out->colormap[Y * r_out->w + X] = *color_out;
      }
    }
  }
}

void tsfx_outline_rotor_tick(RenderSurface_t *r, int start_x,
                                  int start_y, TS_DIRECTION dir) {
  TS_DIRECTION direction = dir;

  Pixel2D_t p_init;
  Pixel2D_t p_current;
  Pixel2D_t p_next;
  Pixel2D_t p_tmp;

  // save xyc of start, get current color
  p_init.x = start_x;
  p_init.y = start_y;
  render_surface_get_color(r, &p_init);
  p_current = p_init;
  p_next = p_current;

  int stop = 0;
  while (!stop) {
    if (direction == TS_DIRECTION_LEFT) {
      p_next.x--;
      if (!render_surface_has_color_thr(r, &p_next, 3)) { // also checks x < 0
        p_next.x++;
        if (render_surface_has_color_thr(r, p_current.x, p_current.y + 1, 3)) {
          direction = TS_DIRECTION_DOWN;
          continue;
        }
        if (render_surface_has_color_thr(r, p_current.x, p_current.y - 1, 3)) {
          direction = TS_DIRECTION_UP;
          continue;
        }
        stop = 1;
        continue;
      }
    } else if (direction == TS_DIRECTION_DOWN) {
      p_next.y++;
      if (!render_surface_has_color_thr(r, &p_next, 3)) { // also checks y > h
        p_next.y--;
        if (render_surface_has_color_thr(r, p_current.x + 1, p_current.y, 3)) {
          direction = TS_DIRECTION_RIGHT;
          continue;
        }
        if (render_surface_has_color_thr(r, p_current.x - 1, p_current.y, 3)) {
          direction = TS_DIRECTION_LEFT;
          continue;
        }
        stop = 1;
        continue;
      }
    } else if (direction == TS_DIRECTION_RIGHT) {
      p_next.x++;
      if (!render_surface_has_color_thr(r, &p_next, 3)) { // also checks x > w
        p_next.x--;
        if (render_surface_has_color_thr(r, p_current.x, p_current.y + 1, 3)) {
          direction = TS_DIRECTION_DOWN;
          continue;
        }
        if (render_surface_has_color_thr(r, p_current.x, p_current.y - 1, 3)) {
          direction = TS_DIRECTION_UP;
          continue;
        }
        stop = 1;
        continue;
      }
    } else if (direction == TS_DIRECTION_UP) {
      p_next.y--;
      if (!render_surface_has_color_thr(r, &p_next, 3)) { // also checks y < 0
        p_next.y++;
        if (render_surface_has_color_thr(r, p_current.x + 1, p_current.y, 3)) {
          direction = TS_DIRECTION_RIGHT;
          continue;
        }
        if (render_surface_has_color_thr(r, p_current.x - 1, p_current.y, 3)) {
          direction = TS_DIRECTION_LEFT;
          continue;
        }
        stop = 1;
        continue;
      }
    }

    render_surface_get_color(r, &p_next);
    p_current.x = p_next.x;
    p_current.y = p_next.y;
    render_surface_set_color(r, &p_current);
    p_current = p_next;
    if ((p_current.x == p_init.x) && (p_current.y == p_init.y))
      stop = 1;
  }
}

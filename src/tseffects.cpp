// tseffects.cpp - libtsprites, 2023-24 M64

#include "include/tseffects.hpp"
#include "include/tscolors.hpp"
#include "include/tsrendersurface.hpp"

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
               RGBColor_t *color_out, unsigned char c, RenderSurface_t *r_out) {
  for (int Y = 0; Y < r_in->h; Y++) {
    for (int X = 0; X < r_in->w; X++) {
      if (rgb_equal(color_in, color_out)) {
        r_out->colormap[Y * r_out->w + X] = *color_out;
      }
    }
  }
}

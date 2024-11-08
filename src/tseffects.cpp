// tseffects.cpp - libtsprites, 2023-24 M64

#include "include/tseffects.hpp"
#include "include/tscolors.hpp"
#include "include/tsrendersurface.hpp"
#include "include/tstypes.h"
#include <math.h>
#include <stdlib.h> // Needed for srand()
#include <time.h>   // Needed for time()
#include <unistd.h>

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

void tsfx_dim_bgcolor(RenderSurface_t *r_in, int step, int maxsteps,
                      RenderSurface_t *r_out, RGBColor_t c) {
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

      dr += c.r;
      dg += c.g;
      db += c.b;

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

void tsfx_outline_rotor_tick(RenderSurface_t *r, int start_x, int start_y,
                             TS_DIRECTION dir) {
  TS_DIRECTION direction = dir;

  Pixel2D_t p_init;
  Pixel2D_t p_current;
  Pixel2D_t p_next;

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

// Update the palette to create a circular blue-to-cyan gradient
void tsfx_plasma_init_palette(TSFXPlasmaCTX_t *pctx) {
  for (int i = 0; i < pctx->palette.num_colors; i++) {
    // Scale to half the palette for a circular effect
    float t = (float)i / ((float)pctx->palette.num_colors / 2);

    if (i < pctx->palette.num_colors / 2) {
      // First half of the palette: dark blue to cyan
      pctx->palette.colors[i].r =
          (int)(30 * t); // Red: slight increase for cyan
      pctx->palette.colors[i].g =
          (int)(200 * t); // Green: significant increase for cyan
      pctx->palette.colors[i].b =
          (int)(150 + 100 * t); // Blue: high starting point, softens for cyan
    } else {
      // Second half of the palette: cyan back to dark blue
      t = (float)(i - (float)pctx->palette.num_colors / 2) /
          ((float)pctx->palette.num_colors / 2); // Normalize t for second half
      pctx->palette.colors[i].r = (int)(30 * (1 - t)); // Red: decreases
      pctx->palette.colors[i].g =
          (int)(200 * (1 - t)); // Green: decreases for blue
      pctx->palette.colors[i].b =
          (int)(150 + 100 * (1 - t)); // Blue returns to high
    }
  }
}

void tsfx_plasma_generate_plasma(TSFXPlasmaCTX_t *pctx, RenderSurface_t *ro) {
  RGBColor_t c_pixel;
  float angle = pctx->time_offset * 0.02; // Angle for rotation effect
  float breathing =
      sin(pctx->time_offset * 0.05) * 0.5 + 1.5; // Breathing effect
  int palette_shift = pctx->time_offset %
                      pctx->palette.num_colors; // Shift the palette over time

  for (int y = 0; y < pctx->heigth; y++) {
    for (int x = 0; x < pctx->width; x++) {
      // Multi-directional pattern using x and y
      float nx = x * cos(angle) - y * sin(angle); // Rotating x
      float ny = x * sin(angle) + y * cos(angle); // Rotating y

      // Balanced sine/cosine patterns for smooth blending
      float value =
          sin(nx * 0.12 + pctx->time_offset * 0.11) +
          cos(ny * 0.13 + pctx->time_offset * 0.09) +
          sin((nx + ny) * 0.1 + pctx->time_offset * 0.08) +
          cos(sqrt(nx * nx + ny * ny) * 0.07 + pctx->time_offset * 0.06) +
          sin((nx - ny) * 0.08);

      // Apply breathing effect for dynamic appearance
      value *= breathing;

      // Smooth out values with tanh for softer transitions
      // Scale to [-CLAMP_RANGE, CLAMP_RANGE]
      // with tanh for gentler transitions
      value = tanh(value) * pctx->clamp_range;

      // Map value to a palette index and loop back
      int color_index =
          (int)((value + pctx->clamp_range) * (pctx->palette.num_colors - 1) /
                    (2 * pctx->clamp_range) +
                palette_shift) %
          pctx->palette.num_colors;

      c_pixel = pctx->palette.colors[color_index];

      // set rgb pixel to surface x, y
      if (pctx->rs->shadowmap[pctx->rs->w * y + x] == PLASMA_REPLACE) {
        ro->colormap[pctx->rs->w * y + x] = c_pixel;
      }
    }
  }
}

void tsfx_plasma_init(TSFXPlasmaCTX_t *pctx, int width, int height,
                      int num_colors, float clamp_range,
                      RGBColor_t replace_color, RenderSurface_t *rs) {
  srand(time(0));

  pctx->width = width;
  pctx->heigth = height;
  pctx->clamp_range = clamp_range;
  pctx->replace_color = replace_color;
  pctx->rs = rs;
  pctx->time_offset = 0;

  RGBColor_t c_chk;
  for (int y = 0; y < rs->h; y++) {
    for (int x = 0; x < rs->w; x++) {
      c_chk = rs->colormap[rs->w * y + x];
      if ((c_chk.r == replace_color.r) && (c_chk.g == replace_color.g) &&
          (c_chk.b == replace_color.b)) {
        rs->shadowmap[rs->w * y + x] = PLASMA_REPLACE;
      }
    }
  }

  InitPalette(&pctx->palette, num_colors); // use 256 colors
  tsfx_plasma_init_palette(pctx);
}

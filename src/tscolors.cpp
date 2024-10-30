// tscolors.cpp - libtsprites, 2023-24 M64

#include "include/tscolors.hpp"
#include "include/tsutils.hpp"

void rgb2hsl(RGBColor_t *rgb, HSLColor_t *hsl) {
  float r = (rgb->r / 255.0f);
  float g = (rgb->g / 255.0f);
  float b = (rgb->b / 255.0f);

  float Min = min(min(r, g), b);
  float Max = max(max(r, g), b);
  float delta = Max - Min;

  hsl->l = (Max + Min) / 2;

  if (delta == 0) {
    hsl->h = 0;
    hsl->s = 0.0f;
  } else {
    hsl->s =
        (hsl->l <= 0.5) ? (delta / (Max + Min)) : (delta / (2 - Max - Min));

    float hue;

    if (r == Max) {
      hue = ((g - b) / 6) / delta;
    } else if (g == Max) {
      hue = (1.0f / 3) + ((b - r) / 6) / delta;
    } else {
      hue = (2.0f / 3) + ((r - g) / 6) / delta;
    }

    if (hue < 0)
      hue += 1;
    if (hue > 1)
      hue -= 1;

    hsl->h = (int)(hue * 360);
  }
}
// Example
//
// RGBColor_t = { 82, 0, 87 };
//
// H: 296
// S: 1
// L: 0.17058824f

// --

float hue2rgb(float v1, float v2, float vH) {
  if (vH < 0)
    vH += 1;

  if (vH > 1)
    vH -= 1;

  if ((6 * vH) < 1)
    return (v1 + (v2 - v1) * 6 * vH);

  if ((2 * vH) < 1)
    return v2;

  if ((3 * vH) < 2)
    return (v1 + (v2 - v1) * ((2.0f / 3) - vH) * 6);

  return v1;
}

void hsl2rgb(HSLColor_t *hsl, RGBColor_t *rgb) {
  if (hsl->s == 0) {
    rgb->r = rgb->g = rgb->b = (unsigned char)(hsl->l * 255);
  } else {
    float v1, v2;
    float hue = (float)hsl->h / 360;

    v2 = (hsl->l < 0.5) ? (hsl->l * (1 + hsl->s))
                        : ((hsl->l + hsl->s) - (hsl->l * hsl->s));

    v1 = 2 * hsl->l - v2;

    rgb->r = (unsigned char)(255 * hue2rgb(v1, v2, hue + (1.0f / 3)));
    rgb->g = (unsigned char)(255 * hue2rgb(v1, v2, hue));
    rgb->b = (unsigned char)(255 * hue2rgb(v1, v2, hue - (1.0f / 3)));
  }
}
// Example
//
// HSLColor_t = { 138, 0.50f, 0.76f };
//
// R: 163
// G: 224
// B: 181

RGBPalette_t *CreatePaletteFadeIn(RGBColor_t c, int steps) {
  RGBPalette_t *p = new RGBPalette_t;
  p->colors = new RGBColor_t[steps];
  p->num_colors = steps;

  double r_inc = (double)c.r / (double)steps;
  double g_inc = (double)c.g / (double)steps;
  double b_inc = (double)c.b / (double)steps;

  RGBColor_t tmpc = {0, 0, 0};

  for (int i = 0; i < steps; i++) {
    tmpc.r = (r_inc * (double)i);
    tmpc.g = (g_inc * (double)i);
    tmpc.b = (b_inc * (double)i);
    p->colors[i] = tmpc;
  }

  return p;
}

RGBPalette_t *CreatePaletteFadeout(RGBColor_t c, int steps) {
  RGBPalette_t *p = new RGBPalette_t;
  p->colors = new RGBColor_t[steps];
  p->num_colors = steps;

  double r_inc = (double)c.r / (double)steps;
  double g_inc = (double)c.g / (double)steps;
  double b_inc = (double)c.b / (double)steps;

  RGBColor_t tmpc = c;

  for (int i = 0; i < steps; i++) {
    tmpc.r = c.r - (r_inc * (double)(steps - i));
    tmpc.g = c.g - (g_inc * (double)(steps - i));
    tmpc.b = c.b - (b_inc * (double)(steps - i));
    p->colors[i] = tmpc;
  }

  return p;
}

RGBPalette_t *CreatePaletteFadeInOut(RGBColor_t c, int steps) {
  RGBPalette_t *p = new RGBPalette_t;
  p->colors = new RGBColor_t[steps];
  p->num_colors = steps;

  double r_inc = (double)c.r / (double)(steps / 2.0);
  double g_inc = (double)c.g / (double)(steps / 2.0);
  double b_inc = (double)c.b / (double)(steps / 2.0);

  RGBColor_t tmpc = {0, 0, 0};

  for (int i = 0; i < (steps / 2); i++) {
    tmpc.r = (r_inc * (double)(i));
    tmpc.g = (g_inc * (double)(i));
    tmpc.b = (b_inc * (double)(i));
    p->colors[i] = tmpc;
  }

  for (int i = (steps / 2); i < steps; i++) {
    tmpc.r = c.r - (r_inc * (double)(i / 2.0));
    tmpc.g = c.g - (g_inc * (double)(i / 2.0));
    tmpc.b = c.b - (b_inc * (double)(i / 2.0));
    p->colors[i] = tmpc;
  }

  return p;
}

RGBPalette_t *CreatePaletteFadeColors(RGBColor_t *c1, RGBColor_t *c2,
                                      int steps) {
  RGBPalette_t *p = new RGBPalette_t;
  p->colors = new RGBColor_t[steps];
  p->num_colors = steps;

  double r_inc = (double)(c2->r - c1->r) / (double)steps;
  double g_inc = (double)(c2->g - c1->g) / (double)steps;
  double b_inc = (double)(c2->b - c1->b) / (double)steps;

  RGBColor_t tmpc;

  for (int i = 0; i < steps; i++) {
    tmpc.r = c1->r + (r_inc * (double)(steps - i));
    tmpc.g = c1->g + (g_inc * (double)(steps - i));
    tmpc.b = c1->b + (b_inc * (double)(steps - i));
    p->colors[i] = tmpc;
  }

  return p;
}

bool rgb_equal(RGBColor_t *c1, RGBColor_t *c2) {
  return (c1->r == c2->r) && (c1->g == c2->g) && (c1->b == c2->b);
}


RGBPalette_t *PaletteAppend(RGBPalette_t *p1, RGBPalette_t *p2)
{

  RGBPalette_t *p = new RGBPalette_t;
  p->num_colors = p1->num_colors + p2->num_colors;
  p->colors = new RGBColor_t[p->num_colors];

  for(int i=0; i<p1->num_colors; i++) {
    p->colors[i] = p1->colors[i];
  }
  for(int i=0; i<p2->num_colors; i++) {
    p->colors[p1->num_colors + i] = p2->colors[i];
  }

  return p;
}

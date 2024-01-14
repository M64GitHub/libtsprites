#include "include/tscolors.hpp"
#include "include/tsutils.hpp"
#include <stdio.h>

void rgb2hsl(rgb_color *rgb, hsl_color *hsl) {
  float r = (rgb->r / 255.0f);
  float g = (rgb->g / 255.0f);
  float b = (rgb->b / 255.0f);

  float Min = min(min(r, g), b);
  float Max = max(max(r, g), b);
  float delta = Max - Min;

  hsl->l = (Max + Min) / 2; // L

  if (delta == 0) {
    hsl->h = 0;    // H
    hsl->s = 0.0f; // S
  } else {
    hsl->s = (hsl->l <= 0.5) ? 
                 (delta / (Max + Min))
                             : (delta / (2 - Max - Min));

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

    hsl->h = (int)(hue * 360); // H
  }
}

// Example
//
// rgb_color = { 82, 0, 87 };
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

void hsl2rgb(hsl_color *hsl, rgb_color *rgb) {
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
// hsl_color = { 138, 0.50f, 0.76f };
//
// R: 163
// G: 224
// B: 181

// tscolors.hpp - libtsprites, 2023-24 M64

#ifndef TSL_COLORS_H
#define TSL_COLORS_H

typedef struct RGBColor_s {
    int r;
    int g;
    int b;
} RGBColor_t;

typedef struct HSLColor_s {
	int h;
	float s;
	float l;
} HSLColor_t;

typedef struct RGBColorPalette_s {
    int num_colors=0;
    RGBColor_t *colors;
} RGBPalette_t;

void  rgb2hsl(RGBColor_t *rgb, HSLColor_t *hsl);
float hue2rgb(float v1, float v2, float vH);
void  hsl2rgb(HSLColor_t *hsl, RGBColor_t *rgb);

RGBPalette_t *CreatePaletteFadeIn(RGBColor_t c, int steps);
RGBPalette_t *CreatePaletteFadeOut(RGBColor_t c, int steps);
RGBPalette_t *CreatePaletteFadeInOut(RGBColor_t c, int steps);

// TODO: Fade from color1 to color2

#endif

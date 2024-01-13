#ifndef TS_COLORS_H
#define TS_COLORS_H

typedef struct s_rgbcolor {
    int r;
    int g;
    int b;
} rgb_color;

typedef struct s_struct_hslcolor {
	int h;
	float s;
	float l;
} hsl_color;

typedef struct s_rgbcolor_palette {
    int num_colors=0;
    rgb_color colors[];
} rgb_palette;

void  rgb2hsl(rgb_color *rgb, hsl_color *hsl);
float hue2rgb(float v1, float v2, float vH);
void  hsl2rgb(hsl_color *hsl, rgb_color *rgb);

#endif


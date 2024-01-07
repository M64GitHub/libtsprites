#ifndef TS_COLORS_H
#define TS_COLORS_H

typedef struct s_rgbcolor {
    int r;
    int g;
    int b;
} rgb_color;

typedef struct s_rgbcolor_palette {
    int num_colors=0;
    rgb_color colors[];
} rgb_palette;

#endif


#ifndef TS_RENDER_H
#define TS_RENDER_H

#include "tscolors.hpp"

typedef struct s_render_surface {
    int w;
    int h;
    int rel_x;
    int rel_y;
    rgb_color     *colormap;
    unsigned char *shadowmap;
} render_surface;
    

#endif


#ifndef TSL_RENDER_H
#define TSL_RENDER_H

#include "tscolors.hpp"

typedef struct s_render_surface {
    char flatmode = 0; // 0 / 1: flat-mode or not flat-mode

    int w; // measured in blocks, 1 ASCI char has width 1
    int h; // measured in blocks, 1 ASCII char has height 2, 1 block height 1
    int z; // layer - for sorting by rendering engine

    rgb_color     *colormap;  // to be rendered on, when flatmode = 0
    unsigned char *shadowmap; // to be rendered on, when flatmode = 0 

    char *s = 0;   // string representation for fast printf
} render_surface;
    
#endif


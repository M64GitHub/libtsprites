// tsprites.cpp, 2023 M64

#include <stdlib.h>
#include "tsprites.h"

TSprite::TSprite()
{
    w = h = x = y = z = 0;
    shadow_map = 0;
    color_map = 0;

    // initialize lines array
    for(int l=0; l<128; l++) lines[l] = 0;
}

TSprite::TSprite(int ww, int hh)
{
    x = y = z = 0;
    shadow_map = 0;
    color_map = 0;

    w = ww;
    h = hh;

    for(int l=0; l<h; l++) lines[l] = 0;

    if(!malloc_maps()) { w = 0; h = 0; return; }
}

TSprite::TSprite(char *imgstr)
{
}

TSprite::~TSprite()
{
    free_maps();
}

int TSprite::ImportFromImgStr(char *s)
{
    return 0;
}

int TSprite::ImportFromFile(char *fn)
{
    return 0;
}

int TSprite::malloc_maps()
{
    shadow_map = (char *) calloc(w * h, 1);
    if(!shadow_map) return 1;

    color_map = (char *) calloc(w * h * 3, 1); // r, g, b
    if(!color_map) return 1;

    return 0;
}

int TSprite::free_maps()
{
    if(!shadow_map) return 1;
    free(shadow_map);

    if(!color_map) return 1;
    free(color_map);

    return 0;
}


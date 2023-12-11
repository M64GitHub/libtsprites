// tsprites.cpp, 2023 M64

#include <stdlib.h>
#include "tsprites.h"

TSprite::TSprite()
{
    w = h = x = y = z = 0;
    shadow_map = 0;
    color_map = 0;
    shadow_map_source = 0;
    color_map_source = 0;
    maps_initialized = 0;

    // initialize lines array
    for(int l=0; l<128; l++) lines[l] = 0;
}

TSprite::TSprite(int ww, int hh)
{
    x = y = z = 0;
    shadow_map = 0;
    color_map = 0;
    shadow_map_source = 0;
    color_map_source = 0;
    maps_initialized = 0;

    for(int l=0; l<h; l++) lines[l] = 0;

    if(!malloc_maps()) return; 

    // only on success
    w = ww;
    h = hh;
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

void TSprite::Print()
{
}

void TSprite::Reset()
{
    if(!maps_initialized) return;


}

int TSprite::malloc_maps()
{
    shadow_map = (char *) calloc(w * h, 1);
    if(!shadow_map) return 1;

    color_map = (char *) calloc(w * h * 3, 1); // r, g, b
    if(!color_map) return 1;

    shadow_map_source = (char *) calloc(w * h, 1);
    if(!shadow_map_source) return 1;

    color_map_source = (char *) calloc(w * h * 3, 1); // r, g, b
    if(!color_map_source) return 1;

    maps_initialized = 1;

    return 0;
}

int TSprite::free_maps()
{
    maps_initialized = 0;

    if(s_source) free(s_source);

    if(!shadow_map) return 1;
    free(shadow_map);

    if(!color_map) return 1;
    free(color_map);

    if(!shadow_map_source) return 1;
    free(shadow_map);

    if(!color_map_source) return 1;
    free(color_map_source);

    return 0;
}


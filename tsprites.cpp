// tsprites.cpp, 2023 M64

#include <stdio.h>
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

    for(int l=0; l<hh; l++) lines[l] = 0;

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

// I: filename
int TSprite::ImportFromFile(char *fn)
{
    FILE *f;
    unsigned long f_size;

    if(!(f = fopen(fn, "rb"))) {
        printf("ERROR: can not open file.\n");
        return 1;
    }

    // -- get size
    fseek(f, 0, SEEK_END);
    f_size = ftell(f);




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
    if(!color_map) { free_maps(); return 1; }

    shadow_map_source = (char *) calloc(w * h, 1);
    if(!shadow_map_source) { free_maps(); return 1; }

    color_map_source = (char *) calloc(w * h * 3, 1); // r, g, b
    if(!color_map_source) { free_maps(); return 1; }

    maps_initialized = 1; // all or none

    return 0;
}

int TSprite::free_maps()
{
    if(s_source) free(s_source);
    if(shadow_map) free(shadow_map);
    if(color_map) free(color_map);
    if(shadow_map_source) free(shadow_map);
    if(color_map_source) free(color_map_source);

    maps_initialized = 0;

    return 0;
}


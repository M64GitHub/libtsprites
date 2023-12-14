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
    s_source = 0;

    DBG ("[TS] TSprite\n");
}

TSprite::TSprite(int ww, int hh)
{
    DBG ("[TS] TSprite(%d, %d)\n", ww, hh);
    x = y = z = 0;
    shadow_map = 0;
    color_map = 0;
    shadow_map_source = 0;
    color_map_source = 0;
    maps_initialized = 0;
    s_source = 0;

    if(!malloc_maps()) return; 

    // only on success
    w = ww;
    h = hh;
    DBG ("[TS] TSprite(%d, %d) initialized\n", w, h);
}

TSprite::TSprite(char *imgstr)
{
    w = h = x = y = z = 0;
    shadow_map = 0;
    color_map = 0;
    shadow_map_source = 0;
    color_map_source = 0;
    maps_initialized = 0;
    s_source = 0;

    DBG ("[TS] TSprite(imgstr)\n");
    // ImportFromImgStr(imgstr);
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
    DBG ("[TS][ImportFromFile]\n");
    FILE *f;
    unsigned long f_size;
    char tmpbuf[16];
    char hdr[] = { 0x1b,0x5b,0x73,0x1b,0x5b,0x3f,0x32,0x35,0x6c };
    int chk = 0;

    if(!(f = fopen(fn, "rb"))) {
        printf("[TS][ImportFromFile] ERROR: can not open file.\n");
        return 1;
    }

    // -- get size
    fseek(f, 0, SEEK_END);
    f_size = ftell(f);
    if(f_size < 9) {
        printf("[TS][ImportFromFile] ERROR: invalid file type! 1\n");
        fclose(f);
        return 1;
    }
    DBG ("[TS][ImportFromFile] file-size: %ld\n", f_size);

    // -- check file "hdr": catimg esc seq 0x1b, 0x5b, 0x73 = "\x1b[s"
    fseek(f, 0, SEEK_SET);
    for(int i=0; i<16; i++) tmpbuf[i]=0x0;
    if(fread(tmpbuf, 1, 9, f) != 9) {
        printf("[TS][ImportFromFile] ERROR: invalid file type! 2\n");
        fclose(f);
        return 1;
    }
    chk=1;
    for(int i=0; i<9; i++) {
        if(tmpbuf[i] != hdr[i]) {
            chk = 0; break;
        }
    }
    if(!chk) {
        printf("[TS][ImportFromFile] ERROR: invalid file type! 3\n");
        fclose(f);
        return 1;
    }
    DBG ("[TS][ImportFromFile] file-header OK!\n");

    // -- start read / convert

    // ImportFromImgStr...

    return 0;
}

int TSprite::ExportImgStr(char *fn, int format)
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
    if(shadow_map_source) free(shadow_map_source);
    if(color_map_source) free(color_map_source);
    if(s_source) free(s_source);

    maps_initialized = 0;

    return 0;
}


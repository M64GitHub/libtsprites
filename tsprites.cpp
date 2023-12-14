// tsprites.cpp, 2023 M64

#include <stdio.h>
#include <stdlib.h>
#include "tsprites.hpp"
#include "format_catimg.hpp"

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

int TSprite::ImportFromImgStr(char *s, int l)
{
    char hdr[] = { CATIMG_HDR };
    int chk = 0;

    // -- check file "hdr": catimg esc seq 0x1b, 0x5b, 0x73 = "\x1b[s"
    chk = 1;
    for(int i=0; i < (int)sizeof(CATIMG_HDR); i++) {
        if(s[i] != hdr[i]) {
            chk = 0; break;
        }
    }
    if(!chk) {
        printf("[TS][ImportFromImgStr] ERROR: invalid file type! 3\n");
        return 1;
    }
    DBG ("[TS][ImportFromImgStr] file-header OK!\n");

    return 0;
}

// I: filename
int TSprite::ImportFromFile(char *fn)
{
    FILE *f;
    unsigned long f_size;
    char *file_contents = 0;

    if(!(f = fopen(fn, "rb"))) {
        printf("[TS][ImportFromFile] ERROR: can not open file.\n");
        return 1;
    }

    // -- get size
    fseek(f, 0, SEEK_END);
    f_size = ftell(f);
    if(f_size < sizeof(CATIMG_HDR)) {
        printf("[TS][ImportFromFile] ERROR: invalid file type! 1\n");
        fclose(f);
        return 1;
    }
    fseek(f, 0, SEEK_SET);
    DBG ("[TS][ImportFromFile] file-size: %ld bytes\n", f_size);

    // -- max file size chk
    if(f_size > CATIMG_MAXSIZE) {
        printf("[TS][ImportFromFile] ERROR: invalid file! (too large)\n");
        fclose(f);
        return 1;
    }

    // -- read file
    file_contents = (char *) calloc(1, f_size);
    if(file_contents == NULL) {
        printf("[TS][ImportFromFile] ERROR: unable to allocate memory for file.\n");
        fclose(f);
        return 1;
    }
    for(unsigned long i=0;i<f_size;i++) {
        if(!fread(file_contents+i, 1, 1, f)) {
            printf("[TS][ImportFromFile] ERROR: unable to read file.\n");
            free(file_contents);
            fclose(f);
            return 1;
        }
    }
    DBG ("[TS][ImportFromFile] successfully read file into buffer.\n");

    // ImportFromImgStr...
    if(ImportFromImgStr(file_contents, f_size)) {
            printf("[TS][ImportFromFile] ERROR: unable to convert file.\n");
            free(file_contents);
            fclose(f);
            return 1;
    }

    free(file_contents);
    fclose(f);

    DBG ("[TS][ImportFromFile] file successfully imported!\n");

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


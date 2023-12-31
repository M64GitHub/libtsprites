// tsprites.cpp, 2023 M64

#include <stdio.h>
#include <stdlib.h>
#include "include/tsprites.hpp"
#include "include/format_catimg.hpp"


// -- TSprite -----------------------------------------------------------------
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

TSprite::TSprite(char *imgstr, int strlen)
{
    w = h = x = y = z = 0;
    shadow_map = 0;
    color_map = 0;
    shadow_map_source = 0;
    color_map_source = 0;
    maps_initialized = 0;
    s_source = 0;

    DBG ("[TS] TSprite(imgstr, strlen)\n");

    if(ImportFromImgStr(imgstr, strlen)) {
        printf("[TS] ERROR: unable to convert imgstr.\n");
    }
}

TSprite::~TSprite()
{
    free_maps();
}

int TSprite::ImportFromImgStr(char *str, int l)
{
    char hdr[] = { CATIMG_HDR };
    int chk = 0;

    // -- check file "hdr": catimg esc seq 0x1b, 0x5b, 0x73 = "\x1b[s"
    if(l < CATIMG_HDR_LEN) {
        printf("[TS][ImportFromImgStr] ERROR: invalid file type! 1\n");
        return 1;
    }
    chk = 1;
    for(int i=0; i < CATIMG_HDR_LEN; i++) {
        if(str[i] != hdr[i]) {
            chk = 0; break;
        }
    }
    if(!chk) {
        printf("[TS][ImportFromImgStr] ERROR: invalid file type! 3\n");
        return 1;
    }
    DBG ("[TS][ImportFromImgStr] file-header OK!\n");

    // start conversion
    int pos = CATIMG_HDR_LEN; // start of 1st line
    int lnr = 0;
    char *tmpbuf = 0; // imported, ready2print char * string, with relative
                      // line ends
    char buf8k[8192]; // 8k buf 

    tmpbuf = (char *) calloc(1, l + 4096); // surplus for new lineend encodings
                                           // 0x0a -> esc: go w left, 1 down 
    if(!tmpbuf) {
        printf("[TS][ImportFromImgStr] ERROR: unable to alloc tmp mem!\n");
        return 1;
    } else {
        DBG ("[TS][ImportFromImgStr] pre-allocated %d bytes for s\n", l+4096);
    }

    // convert line by line
    int lpos    = 0; // pos in line
    int pxcount = 0;
    unsigned int out_idx = 0;

    while( pos < (l - (CATIMG_LINE_END_LEN+CATIMG_FILE_END_LEN)) ) {
        lpos = 0;    // start of line;
        // search end of line
        // s[pos] = start of line
        while(str[pos + lpos + CATIMG_LINE_END_LEN-1] != 0x0a) {
            unsigned char c = str[pos + lpos];    

            if(c == 0x96) {
                // 0x80 = upper half block, 0x84 = lower half block
                if(!lnr) { w++; }// count on the 1st line only
                pxcount++;
            }
            lpos++;
        }

        // -- here we stand at line end, and have width: 
        // -> add this line + go left + go down to s
        DBG ("\nline nr #%d, pos: %d, llen: %d\n", lnr, pos, lpos);

        // copy line to s
        int i;
        for(i=0; i<lpos; i++) tmpbuf[out_idx++] = str[pos+i]; 
        
        // create new, relative line end (avoiding strncat):

        i=0;
        sprintf(buf8k, "\x1b[0m");  // clear all modes
        while(buf8k[i]) {
            tmpbuf[out_idx++] = buf8k[i++];
        }

        i=0;
        sprintf(buf8k, "\x1b[%dD", w);  // cursor go left(lpos)
        while(buf8k[i]) {
            tmpbuf[out_idx++] = buf8k[i++];
        }

        i=0;
        sprintf(buf8k, "\x1b[%dB",1);  // cursor go down(1)
        while(buf8k[i]) {
            tmpbuf[out_idx++] = buf8k[i++];
        }

        // -- 

        lpos += CATIMG_LINE_END_LEN; 
        
        lnr++; h+=2; // 1 char = 2 blocks high

        pos += lpos;
    }

    // --
    tmpbuf[out_idx] = 0x0;
    s = tmpbuf;

    DBG ("\nw x h = %d x %d = pxcount = %d, tt size of conversion: %d\n", 
           w, h, pxcount, out_idx);

    malloc_maps();

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
    if(f_size < CATIMG_HDR_LEN) {
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
    if(!s) return;
    printf("%s", s);
    printf ("\x1b[0m\n"); 
}

void TSprite::Reset()
{
    if(!maps_initialized) return;
}

void TSprite::Render()
{
    
}

// -- 

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

// -- LSprite -----------------------------------------------------------------
// -- ASprite -----------------------------------------------------------------

// -- SSprite -----------------------------------------------------------------

SSprite::SSprite()
{
    DBG ("[SS] SSprite\n");
}

SSprite::SSprite(char *str)
{
    DBG ("[SS] SSprite(char *s)\n");
    
    int l = mystrlen(str);
    int i;

    SSPriteFrame *F = new SSPriteFrame;
    frames = new SSPriteFrame*[1];
    frames[0] = F;

    char *tmpstr = (char*)calloc(l+1, 1);
    DBG ("[SS] strlen: %d\n", l);
    tmpstr[l] = 0x00; // extra safe
    for(i=0;i<l;i++) tmpstr[i] = str[i];

    frames[0]->s =tmpstr; 
    frames[0]->color.r = 255;
    frames[0]->color.g = 255;
    frames[0]->color.b = 255;

    frame_count = 1;
}

SSprite::SSprite(char **s, int len, rgb_color c)
{
}

SSprite::SSprite(char **s, int len, rgb_palette p)
{
}

SSprite::~SSprite()
{
}

void SSprite::Print()
{
    if(!frame_count) return;

    printf("%s", frames[frame_idx]->s);
}

void SSprite::PrintUncolored()
{
    if(!frame_count) return;

    printf("%s", frames[frame_idx]->s);
}


void SSprite::free_frames()
{
}



// -- helper functions --

void cursor_up(int n) 
{
    printf ("\x1b[%dA", n);
}

void cursor_down(int n) 
{
    printf ("\x1b[%dB", n);
}

void cursor_left(int n) 
{
    printf ("\x1b[%dD", n);
}

void cursor_right(int n) 
{
    printf ("\x1b[%dC", n);
}

void cursor_on()
{
    printf("\x1b[?25h");
}

void cursor_off()
{
    printf("\x1b[?25l");
}

void cursor_reset()
{
    printf ("\x1b[0m\n");   // reset all modes
}

void cursor_home()
{

    printf("\x1b[H");    // home pos
}

void board_init()
{
    printf("\x1b[s");    // save cursor pos
    printf("\x1b[?47h"); // save screen
    printf("\x1b[2J");   // erase entire screen
    printf("\x1b[H");    // home pos
    cursor_off();
}

void board_close()
{
    printf("\x1b[?47l"); // restore screen
    printf("\x1b[u");    // restore cursor pos
    cursor_on();
}

int mystrlen(char *str)
{
    if(!str) return 0;

    int i=0;
    while(str[i++]);

    return i;
}



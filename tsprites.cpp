// tsprites.cpp, 2023 M64

#include <stdio.h>
#include <stdlib.h>
#include "include/tsprites.hpp"
#include "include/format_catimg.hpp"


// -- TSprite -----------------------------------------------------------------
TSprite::TSprite()
{
    DBG ("[TS] TSprite\n");
}

TSprite::TSprite(char *imgstr, int strlen)
{
    DBG ("[TS] TSprite(imgstr, strlen)\n");

    if(ImportFromImgStr(imgstr, strlen)) {
        printf("[TS] ERROR: unable to convert imgstr.\n");
    }
}

TSprite::~TSprite()
{
    free_frames();
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

            if(c == 0x96 || c == 0x20) {
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
    if(s) free(s); // when called from Reset();
    s = tmpbuf;
   
    // Skip, when called from Reset();
    if(!s_source) {
        s_source = strdup(tmpbuf);
        s_source_len = out_idx;
    }

    DBG ("\nw x h = %d x %d = pxcount = %d, tt size of conversion: %d\n", 
           w, h, pxcount, out_idx);

    // -- now we have w, h -> we know image size and can create and fill
    //    maps:
    add_frames(1);
    TSPriteFrame *F = frames[frame_count-1]; // our added frame
    F->colormap = new rgb_color[w * h];
    F->shadow_map = new unsigned char[w * h];
    fill_maps_from_inputstr(str, F);

    // -- fill maps from input string ...

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

void TSprite::Print()
{
    if(!s) return;
    printf("%s", s);
    printf ("\x1b[0m\n"); 
}

void TSprite::Reset()
{
    if(!s || !s_source) return;
    // s will be free'd in ImportFromImgStr
    ImportFromImgStr(s_source, s_source_len);
}

void TSprite::Render()
{
   for(int y = 0; y < h; y++) {
        for(int x=0; x < w; x++) {
            if(frames[0]->shadow_map[y*w+x]) {
                printf("\x1b[38;2;%d;%d;%dm",
                       frames[0]->colormap[y*w+x].r,
                       frames[0]->colormap[y*w+x].g,
                       frames[0]->colormap[y*w+x].b
                       );

                printf("*");
            }
            else printf(" ");
        }
        printf("\n");
    }
}

// -- 

int TSprite::fill_maps_from_inputstr(char *str, TSPriteFrame *F)
{
    unsigned int pos, map_idx, map_x, map_y;
    int res, r1, g1, b1, r2, g2, b2;
    r1 = 0; g1 = 0; b1 = 0;
    r2 = 0; g2 = 0; b2 = 0;
    int count = 0;
    char tmpbuf1k[1024];
    rgb_color upper_color;
    rgb_color lower_color;

    pos = CATIMG_HDR_LEN;
    DBG("PARSING ...\n");

    for(map_y = 0; map_y < (h/2); map_y++) {
    for(map_x = 0; map_x < w; map_x++) {
        // -- case 1: doublepixel
        res = sscanf(str + pos, "\x1b[48;2;%d;%d;%dm\x1b[38;2;%d;%d;%dm\u2584",
                    &r1, &g1, &b1,&r2, &g2, &b2);
        if(res == 6) {
            count++;
            DBG("%d case 1: doublepixel: %d %d %d, %d %d %d\n", pos,
                r1, g1, b1, r2, g2, b2);
            while((unsigned char)str[pos] != 0x84) pos++;
            pos++;
            // write to maps
            upper_color.r = r1; upper_color.g = g1; upper_color.b = b1;
            lower_color.r = r2; lower_color.g = g2; lower_color.b = b2;
            F->colormap[(map_y*2)*w + map_x]   = upper_color;
            F->colormap[(map_y*2+1)*w + map_x] = lower_color;
            F->shadow_map[(map_y*2)*w + map_x]  = 1;
            F->shadow_map[(map_y*2+1)*w + map_x] = 1;
            continue;
        }

        // -- case 2 or 3: 
        //    2: upper pixel, lower transparent: 0x80
        //    3: lower pixel, upper transparent: 0x84
        res = sscanf(str + pos, "\x1b[0;38;2;%d;%d;%dm",
                    &r1, &g1, &b1);
        if(res == 3) {
            count++;
            DBG ("STHG found at pos: %d\n", pos);
            int lookahead;
            int found = 0;
            unsigned char c;
            for(lookahead = 14; lookahead < 25; lookahead++) {
                if((unsigned char)str[pos + lookahead] == 0x84) {
                    found = 0x84;
                    DBG("%d case 3: lower pixel: %d %d %d\n", pos,
                        r1, g1, b1);
                    pos+=lookahead+1;
                    // write to maps 
                    lower_color.r = r1; 
                    lower_color.g = g1; 
                    lower_color.b = b1;
                    upper_color.r = 0; 
                    upper_color.g = 0; 
                    upper_color.b = 0;
                    F->colormap[(map_y*2)*w + map_x]   = upper_color;
                    F->colormap[(map_y*2+1)*w + map_x] = lower_color;
                    F->shadow_map[(map_y*2)*w + map_x]  = 0;
                    F->shadow_map[(map_y*2+1)*w + map_x] = 1;
                    break;
                }
                if((unsigned char)str[pos + lookahead] == 0x80) {
                    found = 0x80;
                    DBG("%d case 2: upper pixel: %d %d %d\n", pos,
                        r1, g1, b1);
                    pos+=lookahead+1;
                    // write to maps 
                    upper_color.r = r1; 
                    upper_color.g = g1; 
                    upper_color.b = b1;
                    lower_color.r = 0; 
                    lower_color.g = 0; 
                    lower_color.b = 0;
                    F->colormap[(map_y*2)*w + map_x]   = upper_color;
                    F->colormap[(map_y*2+1)*w + map_x] = lower_color;
                    F->shadow_map[(map_y*2)*w + map_x]  = 1;
                    F->shadow_map[(map_y*2+1)*w + map_x] = 0;
                    break;
                }
            }
            if(!found) {
                DBG("ERR at %d\n", pos); 
                return 1;
            }
            continue;
        }

        // -- case 4: spc
        if((unsigned char)str[pos + 3] == 0x20) {
            count++;
            DBG("%d case 4: SPACE upper pixel and lower transparent\n", pos);
            pos+=4;
            // write to maps 
            upper_color.r = 0; 
            upper_color.g = 0; 
            upper_color.b = 0;
            lower_color.r = 0; 
            lower_color.g = 0; 
            lower_color.b = 0;
            F->colormap[(map_y*2)*w + map_x]   = upper_color;
            F->colormap[(map_y*2+1)*w + map_x] = lower_color;
            F->shadow_map[(map_y*2)*w + map_x]  = 0;
            F->shadow_map[(map_y*2+1)*w + map_x] = 0;
            continue;
        }
        DBG ("%d NOT FOUND!\n", pos);
    }
    DBG ("count: %d\n", count);
    DBG ("pos: %d\n", pos); 
    pos+=4;
    }

    return 0;
}

int TSprite::add_frames(int n)
{
    if(n < 1) return -1;

    // build new frames
    TSPriteFrame **new_frames = new TSPriteFrame *[frame_count + n];
    
    // copy old frames, if any
    for(int i=0; i<frame_count; i++) new_frames[i] = frames[i];

    // add new frames
    for(int i=0; i<n; i++) {
        TSPriteFrame *F = new TSPriteFrame;
        new_frames[frame_count + i] = F;
    }

    // delete old array if one
    if(frames) free(frames);

    frames = new_frames;
    frame_count = frame_count + n;

    return 0;
}

void TSprite::free_frames()
{
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

SSprite::SSprite(char **strs, int len)
{
    DBG ("[SS] SSprite(char **s)\n");
    
    frames = new SSPriteFrame*[len];
    int i=0;

    for(int j=0; j<len; j++) {
        int l = mystrlen(strs[j]);
        SSPriteFrame *F = new SSPriteFrame;
        frames[j] = F;

        char *tmpstr = (char*)calloc(l+1, 1);
        DBG ("[SS] strlen: %d\n", l);
        tmpstr[l] = 0x00; // extra safe
        for(i=0;i<l;i++) tmpstr[i] = strs[j][i];

        frames[j]->s = tmpstr; 
        frames[j]->color.r = 255;
        frames[j]->color.g = 255;
        frames[j]->color.b = 255;
    }

    frame_count = len;
}

SSprite::SSprite(char **strs, int len, rgb_color c)
{
    DBG ("[SS] SSprite(char **s, len %d, color: %d,%d,%d)\n",
                    len, c.r, c.g, c.b
         );
    
    frames = new SSPriteFrame*[len];
    int i=0;

    for(int j=0; j<len; j++) {
        int l = mystrlen(strs[j]);
        SSPriteFrame *F = new SSPriteFrame;
        frames[j] = F;

        char *tmpstr = (char*)calloc(l+1, 1);
        DBG ("[SS] strlen: %d\n", l);
        tmpstr[l] = 0x00; // extra safe
        for(i=0;i<l;i++) tmpstr[i] = strs[j][i];

        frames[j]->s = tmpstr; 
        frames[j]->color.r = c.r;
        frames[j]->color.g = c.g;
        frames[j]->color.b = c.b;
    }

    frame_count = len;
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
    printf("\x1b[38;2;%d;%d;%dm", frames[frame_idx]->color.r,
                                   frames[frame_idx]->color.g,
                                   frames[frame_idx]->color.b
    );
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

// -- screen -- 

void screen_init()
{
    printf("\x1b[s");    // save cursor pos
    printf("\x1b[?47h"); // save screen
    printf("\x1b[2J");   // erase entire screen
    printf("\x1b[H");    // home pos
    cursor_off();
}

void screen_close()
{
    printf("\x1b[?47l"); // restore screen
    printf("\x1b[u");    // restore cursor pos
    cursor_on();
}

// -- helpers

int mystrlen(char *str)
{
    if(!str) return 0;

    int i=0;
    while(str[i++]);

    return i;
}

int mystrcpy(char *dest, char *src)
{
    if(!dest || !src) return 0;

    int i=0;
    while(src[i]) {
        dest[i]=src[i];
        i++;
    }
    dest[i] = 0x00;

    return i;
}

char *strdup(char *src)
{
    if(!src) return 0;

    int l = mystrlen(src);
    char *tmpstr = (char*)calloc(l+1, 1);
    mystrcpy(tmpstr, src);
    
    return tmpstr;
}


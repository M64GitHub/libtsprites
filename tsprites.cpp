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

// TSprite(char *imgstr)
// initialize from imgstr (catimg format), ie when included via 'xxd -i'
// make sure imgstr is 0x00 terminated!
TSprite::TSprite(char *imgstr)
{
    DBG ("[TS] TSprite(imgstr)\n");

    if(ImportFromImgStr(imgstr)) {
        printf("[TS] ERROR: unable to convert imgstr.\n");
    }
}

TSprite::~TSprite()
{
    free_frames();
}

int TSprite::ImportFromImgStr(char *str)
{
    char hdr[] = { CATIMG_HDR };
    int chk = 0;
    int width = 0;
    int height = 0;
    int l = 0;

    l = mystrlen(str);

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

    // -- start conversion
    int pos = CATIMG_HDR_LEN; // start of 1st line
    int lnr = 0;
    char *outstr = 0; // imported, ready2print char * string, with relative
                      // line ends
    char buf8k[8192]; // 8k buf 

    outstr = (char *) calloc(1, l + 4096); // surplus for new lineend encodings
                                           // 0x0a -> esc: go w left, 1 down 
    if(!outstr) {
        printf("[TS][ImportFromImgStr] ERROR: unable to alloc tmp mem!\n");
        return 1;
    } else {
        DBG ("[TS][ImportFromImgStr] pre-allocated %d bytes for s\n", l+4096);
    }

    // -- convert line by line
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
                if(!lnr) { width++; }// count on the 1st line only
                pxcount++;
            }
            lpos++;
        }

        // -- here we stand at line end, and have width: 
        // -> add this line + go left + go down to s
        DBG ("\nline nr #%d, pos: %d, llen: %d\n", lnr, pos, lpos);

        // copy line to s
        int i;
        for(i=0; i<lpos; i++) outstr[out_idx++] = str[pos+i]; 
        
        // create new, relative line end

        i=0;
        sprintf(buf8k, "\x1b[0m");  // clear all modes
        while(buf8k[i]) {
            outstr[out_idx++] = buf8k[i++];
        }

        i=0;
        sprintf(buf8k, "\x1b[%dD", width);  // cursor go left(lpos)
        while(buf8k[i]) {
            outstr[out_idx++] = buf8k[i++];
        }

        i=0;
        sprintf(buf8k, "\x1b[%dB",1);  // cursor go down(1)
        while(buf8k[i]) {
            outstr[out_idx++] = buf8k[i++];
        }

        // -- 

        lpos += CATIMG_LINE_END_LEN; 
        
        lnr++; height+=2; // 1 char = 2 blocks high

        pos += lpos;
    }
    outstr[out_idx] = 0x0; // conversion done

    DBG ("\nw x h = %d x %d = pxcount = %d, tt px-size of conversion: %d\n", 
           width, height, pxcount, out_idx);

    // -- now we have w, h -> we know image size and can create and fill
    //    maps / a new frame:

    TSPriteFrame *F = add_frames(1, width, height);
    F->s = strdup(outstr);

    // -- fill maps from input string
    imgstr_2maps(str, F);

    // -- create 1down representation, store in frame
    create_1down_str(F);

    // Skip, when called for further frames:
    // Sprite gets initialized only from 1st frame (frame 0)
    if(!frame_count) { 
        s = outstr;
        h = height;
        w = width;
        s_1down = strdup(F->s_1down);
    }

    return 0;
}

// I: filename
int TSprite::ImportFromFile(char *fn)
{
    FILE *f;
    unsigned long f_size;
    char *f_contents = 0;

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
    f_contents = (char *) calloc(1, f_size + 1); // add 1 for 0x00 terminator
    if(f_contents == NULL) {
        printf("[TS][ImportFromFile] ERROR: unable to allocate memory"
               " for file.\n");
        fclose(f);
        return 1;
    }
    for(unsigned long i=0;i<f_size;i++) {
        if(!fread(f_contents+i, 1, 1, f)) {
            printf("[TS][ImportFromFile] ERROR: unable to read file.\n");
            free(f_contents);
            fclose(f);
            return 1;
        }
    }
    f_contents[f_size] = 0x00; // terminate string properly
    DBG ("[TS][ImportFromFile] successfully read file into buffer.\n");

    // ImportFromImgStr... 
    if(ImportFromImgStr(f_contents)) {
            printf("[TS][ImportFromFile] ERROR: unable to convert file.\n");
            free(f_contents);
            fclose(f);
            return 1;
    }

    free(f_contents);
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

void TSprite::Print(int x, int y)
{
    cursor_home();
    cursor_down(y);
    cursor_right(x);

    // if(
}

void TSprite::Render()
{
}

void TSprite::PrintDebugMap(TSPriteFrame *F)
{
    printf("Frame #%d: COLOR:'o', TRANSPARENCY:'-' (with colors) "
           "w x h: %d x %d\nblk ln map\n", F->nr, F->w, F->h);
    for(int y = 0; y < F->h; y++) {
        printf("%02d, %02d ", y, y/2);
        for(int x=0; x < F->w; x++) {
            if(F->shadow_map[y*F->w+x]) {
                printf("\x1b[38;2;%d;%d;%dm",
                       F->colormap[y*F->w+x].r,
                       F->colormap[y*F->w+x].g,
                       F->colormap[y*F->w+x].b
                       );
                printf("o");
            }
            else {
                printf("\x1b[38;2;60;60;60m");
                printf("-");
            }
        }
        printf("\x1b[0m\n");
    }
    printf ("\x1b[0m\n"); 
    printf("Frame #%d: COLOR: 'o', TRANSPARENCY: '-' (without colors) "
           "w x h: %d x %d\nblk ln map\n", F->nr, F->w, F->h);
    for(int y = 0; y < F->h; y++) {
        printf("%02d, %02d ", y, y/2);
        for(int x=0; x < F->w; x++) {
            if(F->shadow_map[y*F->w+x]) {
                printf("o");
            }
            else {
                printf("-");
            }
        }
        printf("\n");
    }
    printf ("\x1b[0m\n"); 
}

// -- 

// TSprite::imgstr_2maps
//
// helper for ImportFromImgStr: catimg line ends!
int TSprite::imgstr_2maps(char *str, TSPriteFrame *F)
{
    unsigned int pos, map_x, map_y;
    int res, r1, g1, b1, r2, g2, b2;
    r1 = 0; g1 = 0; b1 = 0;
    r2 = 0; g2 = 0; b2 = 0;
    int count = 0;
    char tmpbuf1k[1024];
    rgb_color upper_color;
    rgb_color lower_color;

    pos = CATIMG_HDR_LEN;
    DBG("PARSING ...\n");

    for(map_y = 0; map_y < (F->h/2); map_y++) {
    for(map_x = 0; map_x < F->w; map_x++) {
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
            F->colormap  [(map_y*2  ) * F->w + map_x] = upper_color;
            F->colormap  [(map_y*2+1) * F->w + map_x] = lower_color;
            F->shadow_map[(map_y*2  ) * F->w + map_x] = 1;
            F->shadow_map[(map_y*2+1) * F->w + map_x] = 1;
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
                    F->colormap  [(map_y*2  ) * F->w + map_x] = upper_color;
                    F->colormap  [(map_y*2+1) * F->w + map_x] = lower_color;
                    F->shadow_map[(map_y*2  ) * F->w + map_x] = 0;
                    F->shadow_map[(map_y*2+1) * F->w + map_x] = 1;
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
                    F->colormap  [(map_y*2  ) * F->w + map_x] = upper_color;
                    F->colormap  [(map_y*2+1) * F->w + map_x] = lower_color;
                    F->shadow_map[(map_y*2  ) * F->w + map_x] = 1;
                    F->shadow_map[(map_y*2+1) * F->w + map_x] = 0;
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
            F->colormap  [(map_y*2  ) * F->w + map_x] = upper_color;
            F->colormap  [(map_y*2+1) * F->w + map_x] = lower_color;
            F->shadow_map[(map_y*2  ) * F->w + map_x] = 0;
            F->shadow_map[(map_y*2+1) * F->w + map_x] = 0;
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

// create_1down_str(TSPriteFrame *F)
// helper for ImportFromImgStr
// Uses the frame's color / shadow maps to create a position independent
// printable string, having the sprite moved down 1/2 a character.
char *TSprite::create_1down_str(TSPriteFrame *F)
{
    char *tmpstr = (char *)calloc(1, mystrlen(F->s) * 2); // more than enough
    int tmpstr_idx = 0;
    rgb_color upper;
    rgb_color lower;

    // h was initialized with +=2 -> always cleanly divisable
    for(int Y=0; Y < F->h/2; Y++) {
        // if line #0
        if(!Y) {             
            for(int X=0; X < F->w; X++) {
                // assume 1st row transparent
                upper = { 0, 0, 0}; // not required at all
                lower = F->colormap[X];
                // printf a top transparent color of map-row 0, 
                // then line-end
            }
            continue;
        }

        // all else
        for(int X=0; X < F->w; X++) {
            lower = F->colormap[X + (Y*2) * F->w];
            // printf a double color, then line-end
        }

        // if last line: print 1 more depending on shadow map, or not
        if(Y == (F->h/2 - 1)) {
            // when it starts with no pixel, none more will follow, break
            if(!F->shadow_map[(Y*2 + 1)]) break;
            // else do the line with n upper pixel, and last row of 
            // colormap as upper color
        }
    }

    char *outstr = strdup(tmpstr); // copies and truncates properly
    free(tmpstr);                  // so we can free excess memory

    return outstr;
}


// add_frames
// DOES allocate or initialize colormap / shadowmap!
// returns the 1st of appended / new frames
TSPriteFrame *TSprite::add_frames(int n, int width, int height)
{
    if(n < 1) return 0;

    // build new frames
    TSPriteFrame **new_frames = new TSPriteFrame *[frame_count + n];
    
    // copy old frames, if any
    for(int i=0; i<frame_count; i++) new_frames[i] = frames[i];

    // add new frames
    for(int i=0; i<n; i++) {
        TSPriteFrame *F = new TSPriteFrame;
        new_frames[frame_count + i] = F;
        F->nr          = frame_count + i;
        F->colormap    = new rgb_color[width * height];
        F->shadow_map  = new unsigned char[width * height];
        F->w           = width;
        F->h           = height;
        F->s           = 0;
        F->s_1down     = 0;
    }

    // delete old array if one
    if(frames) free(frames);

    frames = new_frames;
    frame_count = frame_count + n;

    return frames[frame_count - n];
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


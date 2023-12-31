#ifndef TSPRITES_H
#define TSPRITES_H

#include "tscolors.hpp"

// #define DEBUG

#ifdef DEBUG
#define DBG(...) fprintf (stderr, __VA_ARGS__)
#else 
#define DBG(...) 
#endif

// True-Color (24bit RGB) unicode based sprite.
// All y msmts and related values (height h) in "half" characters!)
class TSprite
{
public:
    TSprite();
    TSprite(int ww, int hh);
    TSprite(char *imgstr, int strlen);

    ~TSprite();

    int ImportFromImgStr(char *s, int l); // str needs not to be 0x00-terminated
    int ImportFromFile(char *fn);
    int ImportFromColorMap(char *cmap);

    int ExportImgStr(char *fn, int format); // export as .h, txtfile, ...

    void Reset(); // restores original state (after 
                  // possible applied effects)

    void Print(); // output s

    virtual void Render(); // reassemble from maps and render on screen

    // main attributes w/o getters for fastest access
    int w = 0;
    int h = 0; // in blocks / "half characters"
    int x = 0;
    int y = 0; // in blocks / "half characters"
    int z = 0;
    char *s = 0; // for fast printf(), restored after Reset()

    // internal representation - for fast effects processing, ...
    char *shadow_map; // w * h: like alpha channel: 0: no px, 1: px
                      // for collision detection, Print() transparency ...
    char *color_map;  // w * h: r, g, b

private:
    int malloc_maps();
    int free_maps();
    int free_strings();

    int maps_initialized = 0; // either all or none,
                              // for better readibility          

    // copies of initial state
    char *s_source;           // internal, converted representation 
    int   s_source_len;       // for speed, to avoid strlen()
    char *shadow_map_source;  //
    char *color_map_source;   //
};

// class LSprite; // Line-Sprite

// class ASprite; // ASCII Sprite

// SSprite
// Simple- or String-Sprite. Sprite for spinners, and such.
// A collecttion of char *frames, with a fix height of 1 line
// (2 blocks). All characters of a frame have 1 color

typedef struct s_SSPrite_Frame {
    char *s;            // frame content;
    rgb_color color; 
} SSPriteFrame;

class SSprite 
{
public:
    SSprite();
    SSprite(char *str); // 1 frame, framecolor=white 
    SSprite(char **str, int len); // array of strs
    SSprite(char **str, int len, rgb_color c); // array of strs, basecolor
    SSprite(char **str, int len, rgb_palette p); // array of strs, individual colors
    ~SSprite();

    void Print();
    void PrintUncolored();

    int frame_count = 0;
    SSPriteFrame **frames=0;
    int frame_idx = 0; // current frame
private:
    void free_frames();
    unsigned char color_override = 0; // if frame color is unsed,
                                      // or overriden by effect.
};

// -- helper functions

void cursor_up(int n);
void cursor_down(int n);
void cursor_left(int n);
void cursor_right(int n);
void cursor_home();

void cursor_on();
void cursor_off();
void cursor_reset();

void board_init();
void board_close();

int mystrlen(char *s);

#endif


#ifndef TSPRITES_H
#define TSPRITES_H

#include "tscolors.hpp"

// #define DEBUG

#ifdef DEBUG
#define DBG(...) fprintf (stderr, __VA_ARGS__)
#else 
#define DBG(...) 
#endif

// TSprite

typedef struct s_TSpriteFrame {
    
} TSPriteFrame;

// TSprite 
// True-Color (24bit RGB) unicode based sprite.
// All y msmts and related values (height h) in "half" characters!)
class TSprite
{
public:
    TSprite();
    TSprite(int ww, int hh);
    TSprite(char *imgstr, int strlen);

    ~TSprite();

    int ImportFromFile(char *fn);
    int ImportFromImgStr(char *s, int l); 

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

private:
    // copies of initial state
    char *s_source;           // internal, converted representation 
    int   s_source_len;       // for speed, to avoid strlen()

    int  add_frameset(int width, int height, int num);
    void free_frames();

    int frame_count = 0;
    TSPriteFrame **frames=0;
    int frame_idx = 0; // current frame
};

// class LSprite; // Line-Sprite

// class ASprite; // ASCII Sprite


// SSprite

//
typedef struct s_SSPrite_Frame {
    char *s = 0;            // frame content;
    rgb_color color; 
} SSPriteFrame;

// SSprite
// Simple- or String-Sprite. Sprite for spinners, and such.
// A collecttion of char *frames, with a fix height of 1 line
// (2 blocks). All characters of a frame have 1 color
class SSprite 
{
public:
    SSprite();
    SSprite(char *str); // 1 frame, framecolor=white 
    SSprite(char **str, int len); // array of strs
    SSprite(char **str, int len, rgb_color c); // array of strs, basecolor
    SSprite(char **str, int len, rgb_palette p); // array of strs, 
                                                 // individual colors
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

void screen_init();
void screen_close();

int mystrlen(char *s);
int mystrcpy(char *dest, char *src);
char *strdup(char *src);

#endif


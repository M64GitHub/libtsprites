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
  int            nr = 0;
  int            w  = 0;
  int            h  = 0;
  rgb_color     *colormap   = 0;
  unsigned char *shadow_map = 0;
  char          *s = 0;    // copy of (lineend-converted) s
  char          *s_1down;  // copy of (lineend-converted) s_1down
} TSPriteFrame;

typedef struct s_TSpriteAnimation {
    char *name = 0;
    
    int *animation; // array of frame indizes
    int ani_len;
    int *rel_x = 0; // relative x moves
    int *rel_y = 0; // relative y moves

    class TSprite *S = 0; // sprite can reference frames
                          // of other sprites (overlay animations)
} TSpriteAnimation;

// TSprite 
// True-Color (24bit RGB) unicode based sprite.
class TSprite
{
public:
    TSprite();
    TSprite(int ww, int hh);
    TSprite(char *imgstr); // catimg format

    ~TSprite();

    int ImportFromFile(char *fn);
    int ImportFromImgStr(char *s); // catimg format

    void Print(); // printf s
    void Print(int X, int Y); // move cursor, printf s or s_1down
    void PrintFrame(int n); // printf a frame

    virtual void Render(); // reassemble from maps and render 

    void PrintDebugMap(TSPriteFrame *F); // colored map representation

    // main attributes w/o getters for fastest access
    int w = 0;
    int h = 0; // in blocks / "half characters"
    int x = 0;
    int y = 0; // in blocks / "half characters"
    int z = 0;
    char *s = 0; // for fast Print() / printf()
    char *s_1down = 0; // for convenience, created on import: 
        // pre-rendered string-representation, having the sprite 
        // moved 1 block down. For fast Print(x, y) ( using only printf() ),
        // if you don't want to deal with frames / rendering at all.
        // -> Makes smooth Y-movements possible with fast printf();

    // frames for slicing, animations, ...
    int frame_count = 0;     // 1 after Import
    TSPriteFrame **frames=0; // array of pointers
    int frame_idx = 0;       // current frame

    // animations (framesets)
    int ani_count = 0; 
    TSpriteAnimation **animations = 0; // array of pointers

    int counter1 = 0; // convenience counters and thresholds
    int counter2 = 0;
    int counter3 = 0;
    int threshhold1 = 0;
    int threshhold2 = 0;
    int threshhold3 = 0;

    int state       = 0; // generic type to support own concepts

private:
    // allocates maps, returns first new frame 
    TSPriteFrame *add_frames(int n, int width, int height);
    void free_frames();
    
    // import helpers
    int   imgstr_2maps(char *str, TSPriteFrame *F); // part of import
    char *create_1down_str(TSPriteFrame *F); // part of import
  
    rgb_color     *background = 0; // for rendering
};

// class LSprite; // Line-Sprite

// class ASprite; // ASCII Sprite


// SSprite

//
typedef struct s_SSPrite_Frame {
    char *s = 0; // frame content;
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

#endif


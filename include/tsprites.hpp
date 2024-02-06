// tsprites.hpp - libtsprites, 2023-24 M64

#ifndef TSL_TSPRITES_H
#define TSL_TSPRITES_H

#include "tsanimations.hpp"
#include "tsrendersurface.hpp"

// #define DEBUG

#ifdef DEBUG
#define DBG(...) fprintf(stderr, __VA_ARGS__)
#else
#define DBG(...)
#endif

// -- TSprite --

typedef struct s_TSpriteFrame {
  int nr = 0;
  int w = 0;
  int h = 0;
  rgb_color *colormap = 0;      // should stay, to restore b4 effects
  unsigned char *shadowmap = 0; // (apply effects, etc to out_surface)
  char *s = 0;   // frame 0: copy of s (free will not affect spr->s)
  char *s_1down; // frame 0: copy of s_1down

  render_surface *out_surface = 0; // active frame's out_surface will be
  // sprite's out_surface on TSprite::Render()
} TSPriteFrame;

typedef struct s_TFrameSet {
  int frame_count = 0;       // 1 after Import
  TSPriteFrame **frames = 0; // array of pointers to frames
  int frame_idx = 0;         // current frame
} TFrameSet;

//! True-Color (24bit RGB) unicode block character based pixel sprite.

//! Supports printf() style api and rendering based api.
//! starts here.
class TSprite {
public:
  TSprite();
  TSprite(int ww, int hh); // prepares all datastructures
  TSprite(char *imgstr);   // import from catimg format UTF8 string

  ~TSprite();

  int ImportFromFile(char *fn);
  int ImportFromImgStr(char *s); // catimg format

  // -- Split Functions

  int Split(TSPriteFrame *F, int swidth, int sheight); // Split in a fixed
  // vertically  and horizontally raster of cut lines, append created frames to
  // to sprite's fs. Returns index of 1st created new frame.
  int VSplit(TSPriteFrame *F, int swidth);
  // Split and append to sprite's fs. Vertical cut lines, fixed  width. Starts
  // at x=0. Returns index into sprite's fs of first created new frame.
  int VSplit(TSPriteFrame *F, int *swidths, int numslices); // Split and append
  // created frames to sprite's fs. Vertical cut line. You need to specify the
  // size of the "swidths" array in numslices. Variable widths. Starts at
  // x=xoffsets[0]. Returns index into sprite's fs of first new frame.
  // Use to split word-logo into single letters for example.
  int VSplit(TSPriteFrame *F, int *xoffsets, int *widths, int numslices); //
  // Split and append created frames to sprite's fs. Vertical cut line.
  // You need to specify the size of the "swidths" array in numslices. Variable
  // widths. Starts at x=xoffsets[0]. Returns index into sprite's fs of first
  // new frame. Use to split word-logo into single letters for example.

  // Split and return array of newly created TSprite ptrs. Vertical cut line.
  // Variable widths.
  // Starts at x=xoffsets[0].
  // Use to split word-logo into single letters for example.
  TSprite **VSplit2Sprites(TSPriteFrame *F, int *xoffsets, int *widths,
                           int numslices);

  // split and return as new animation
  SpriteAnimation *Split2Ani(TSPriteFrame *F, int swidth, int sheight);
  SpriteAnimation *VSplit2Ani(TSPriteFrame *F, int swidth);
  SpriteAnimation *VSplit2Ani(TSPriteFrame *F, int *swidths, int numslices);

  // String Output
  void Print();             // printf string representation s of first frame
  void Print(int X, int Y); // move cursor, printf s or s_1down
  void PrintFrame(int n);   // printf a specific frame

  virtual void Prepare(); // setup internal animations structures, overlays,
                          // etc, 1 time at start / after creation

  // apply internal animations, etc and render into out_surface
  // return the surface to calling screen
  render_surface *Render();
  void SetXY(int xx, int yy);

  virtual void tick(); // receive global tick(). to handle internal anis, etc

  // control sprite internal animations
  void AddAnimation(SpriteAnimation *a);
  void StartAnimation(int n, int loop);
  void PauseAnimation(int n);
  void StopAnimation(int n);
  void AnimationTick(int n);

  // control single frame animations
  void AddFrameAnimation(SpriteAnimation *a, TSPriteFrame *f);
  void StartFrameAnimation(TSPriteFrame *f, int loop);
  void PauseFrameAnimation(TSPriteFrame *f);
  void StopFrameAnimation(int n);
  void FrameAnimationTick(TSPriteFrame *f);

  // Convert between string and map representation
  int UTF8_2_maps(char *str, TSPriteFrame *F); //
  unsigned char *Maps_2_UTF8(TSPriteFrame *F); //

  void PrintDebugMap(TSPriteFrame *F); // colored map representation

  // main attributes w/o getters for fastest access
  int w = 0;
  int h = 0; // in blocks / "half characters"
  int x = 0;
  int y = 0; // in blocks / "half characters"
  int z = 0;
  TFrameSet fs; // frames for slicing, animations, ...

  // animations
  SpriteAnimation **animations = 0; // array of pointers

  // convenience counters and thresholds
  int counter1 = 0;
  int counter2 = 0;
  int counter3 = 0;
  int threshhold1 = 0;
  int threshhold2 = 0;
  int threshhold3 = 0;

  int state = 0; // generic type to support own concepts

  render_surface *out_surface = 0; // last render, direct access for speed.
  // This points to either the sprite's own render_surface, or is being
  // replaced by ptr to a frame's out_surface.
  render_surface *restore_surface = 0; // original out_surface copy
                                       // before effect, or any manipulation.
private:
  // allocates maps, returns first new frame
  TSPriteFrame *add_frames(int n, int width, int height);
  void free_frames();

  // import helpers
  char *create_1down_str(TSPriteFrame *F);      // part of import
  int imgstr_2maps(char *str, TSPriteFrame *F); // part of import

  char *s = 0; // for fast Print() / printf()
  // for convenience, created on import:
  // pre-rendered string-representation, having the sprite
  // moved 1 block down. For fast Print(x, y) ( using only printf() ),
  // if you don't want to deal with frames / rendering at all.
  // -> Makes smooth Y-movements possible with fast printf();
  char *s_1down = 0;

  rgb_color *background = 0; // for rendering
};

// class LSprite; // Line-Sprite

// class ASprite; // ASCII Sprite

// -- SSprite --

typedef struct s_SSPrite_Frame {
  char *s = 0; // frame content;
  rgb_color color;

  int *animation; // array of frame indizes
  int ani_len = 0;
  int *rel_x = 0; // relative x moves
  int *rel_y = 0; // relative y moves
} SSPriteFrame;

// SSprite
// Simple- or String-Sprite. Sprite for spinners, and such.
// A collecttion of char *frames, with a fix height of 1 line
// (2 blocks).
class SSprite {
public:
  SSprite();
  SSprite(char *str);                          // 1 frame, framecolor=white
  SSprite(char **str, int len);                // array of strs
  SSprite(char **str, int len, rgb_color c);   // array of strs, basecolor
  SSprite(char **str, int len, rgb_palette p); // array of strs,
                                               // individual colors
  ~SSprite();

  void Print();
  void Print(int X, int Y);
  void PrintUncolored();
  void PrintDimmed(); // printf s

  void PrintFrame(int n); // printf a frame

  virtual void Render(); //

  int x = 0;
  int y = 0; // in blocks / "half characters"
  int z = 0;
  char *s = 0; // for fast Print() / printf()

  int frame_count = 0;
  SSPriteFrame **frames = 0;
  int frame_idx = 0; // current frame

  int counter1 = 0; // convenience counters and thresholds
  int counter2 = 0;
  int counter3 = 0;
  int threshhold1 = 0;
  int threshhold2 = 0;
  int threshhold3 = 0;

  rgb_color *background = 0; // for rendering
  int state = 0;             // generic type to support own concepts
private:
  void free_frames();
  unsigned char color_override = 0; // if frame color is unsed,
                                    // or overriden by effect.
};

#endif

// tsprites.hpp - libtsprites, 2023-24 M64

#ifndef TSL_TSPRITES_H
#define TSL_TSPRITES_H

#include "tsanimations.hpp"
#include "tscolors.hpp"
#include "tsrendersurface.hpp"

// #define DEBUG

#ifdef DEBUG
#define DBG(...) fprintf(stderr, __VA_ARGS__)
#else
#define DBG(...)
#endif

// NOTE: TSprite

typedef struct TSpriteFrame_s {
  int rel_x = 0; // relative x position to spr->x
  int rel_y = 0; // relative y position to spr->y
  int nr = 0;
  int w = 0;
  int h = 0;
  RGBColor_t *colormap = 0;      // should stay, to restore b4 effects
  unsigned char *shadowmap = 0; // (apply effects, etc to out_surface)
  char *s = 0;   // frame 0: copy of s (free will not affect spr->s)
  char *s_1down; // frame 0: copy of s_1down

  RenderSurface_t *out_surface = 0; // active frame's out_surface will be
  // sprite's out_surface on TSprite::Render()
} TSpriteFrame_t;

typedef struct TFrameSet_s {
  int frame_count = 0;       // 1 after Import
  TSpriteFrame_t **frames = 0; // array of pointers to frames
  int frame_idx = 0;         // current frame
} TSpriteFrameSet_t;

/// True-Color (24bit RGB) unicode block character based pixel sprite.
//
// More documentation
// needs to  be provided

class TSprite {
public:
  TSprite();
  TSprite(int ww, int hh); ///< prepares all datastructures
  TSprite(char *imgstr);   ///< import from catimg format UTF8 string

  ~TSprite();

  // NOTE: Load/Import Functions

  /// catimg format
  int ImportFromFile(char *fn);
  int ImportFromImgStr(char *s);

  /// PNG
  int ImportFromPNGFile(char *fn);

  /// (same or other's) Sprite frame
  // TODO:
  int ImportFromFrame(TSpriteFrame_t *F);

  // NOTE: Split Functions

  /// Split in a fixed
  // vertically  and horizontally raster of cut lines, append created frames to
  // to sprite's fs. Returns index of 1st created new frame.
  int SplitFixedWH(TSpriteFrame_t *F, int swidth, int sheight);

  /// Split and append to sprite's fs. Vertical cut lines, fixed  width. Starts
  // at x=0. Returns index into sprite's fs of first created new frame.
  int VSplitFixedW(TSpriteFrame_t *F, int swidth);

  /// Split and append
  // created frames to sprite's fs. Vertical cut line. You need to specify the
  // size of the "swidths" array in numslices. Variable widths. Starts at
  // x=xoffsets[0]. Returns index into sprite's fs of first new frame.
  // Use to split word-logo into single letters for example.
  int VSplit(TSpriteFrame_t *F, int *swidths, int numslices);

  /// Split and append created frames to sprite's fs. Vertical cut line.
  // You need to specify the size of the "swidths" array in numslices. Variable
  // widths. Starts at x=xoffsets[0]. Returns index into sprite's fs of first
  // new frame. Use to split word-logo into single letters for example.
  int VSplit(TSpriteFrame_t *F, int *xoffsets, int *widths, int numslices);

  /// Split and return array of newly created TSprite ptrs. Vertical cut line.
  // Variable widths.
  // Starts at x=xoffsets[0].
  // Use to split word-logo into single letters for example.
  // TODO:
  TSprite **VSplit2Sprites(TSpriteFrame_t *F, int *xoffsets, int *widths,
                           int numslices);

  // NOTE: String API
  void Print();             // printf string representation s of first frame
  void Print(int X, int Y); // move cursor, printf s or s_1down
  void PrintFrame(int n);   // printf a specific frame

  // NOTE: Control Functions

  virtual void Prepare(); // setup internal animations structures, overlays,
                          // etc, 1 time at start / after creation

  // apply internal animations, etc and render into out_surface
  // return the surface to calling screen
  RenderSurface_t *Render();
  void SetXY(int xx, int yy);

  virtual void tick(); // receive global tick(). to handle internal anis, etc

  // NOTE: Animation Functions

  // control sprite internal - "direct" animations: 
  // TODO:
  void AddAnimation(TSpriteAnimation_t *a);
  // TODO:
  void StartAnimation(int n, int loop);
  // TODO:
  void PauseAnimation(int n);
  // TODO:
  void StopAnimation(int n);
  // TODO:
  void AnimationTick(int n);

  // NOTE: Conversion / Utility Functions

  // Convert between string and map representation
  int UTF8_2_maps(char *str, TSpriteFrame_t *F); //
  unsigned char *Maps_2_UTF8(TSpriteFrame_t *F); //

  void PrintDebugMap(TSpriteFrame_t *F); // colored map representation

  // main attributes w/o getters for fastest access
  int w = 0;
  int h = 0; // in blocks / "half characters"
  int x = 0;
  int y = 0; // in blocks / "half characters"
  int z = 0;
  TSpriteFrameSet_t fs; // frames for slicing, animations, ...

  // animations
  TSpriteAnimation_t **animations = 0; // array of pointers

  // convenience counters and thresholds
  int counter1 = 0;
  int counter2 = 0;
  int counter3 = 0;
  int threshhold1 = 0;
  int threshhold2 = 0;
  int threshhold3 = 0;

  int state = 0; // generic type to support own concepts

  RenderSurface_t *out_surface = 0; // last render, direct access for speed.
  // This points to either the sprite's own RenderSurface_t, or is being
  // replaced by ptr to a frame's out_surface.
  RenderSurface_t *restore_surface = 0; // original out_surface copy
                                       // before effect, or any manipulation.
private:
  // allocates maps, returns first new frame
  TSpriteFrame_t *add_frames(int n, int width, int height);
  void free_frames();

  // import helpers
  char *create_1down_str(TSpriteFrame_t *F);      // part of import
  int imgstr_2maps(char *str, TSpriteFrame_t *F); // part of import

  char *s = 0; // for fast Print() / printf()

  // for convenience, created on import:
  // pre-rendered string-representation, having the sprite
  // moved 1 block down. For fast Print(x, y) ( using only printf() ),
  // if you don't want to deal with frames / rendering at all.
  // -> Makes smooth Y-movements possible with fast printf();
  char *s_1down = 0;

  RGBColor_t *background = 0; // for rendering
};

// NOTE: LSprite
// class LSprite; // Line-Sprite
// TODO:

// NOTE: ASprite
// class ASprite; // ASCII Sprite
// TODO:

// NOTE: SSprite

typedef struct SSPriteFrame_s {
  char *s = 0; // frame content;
  RGBColor_t color;

  int *animation; // array of frame indizes
  int ani_len = 0;
  int *rel_x = 0; // relative x moves
  int *rel_y = 0; // relative y moves
} SSpriteFrame_t;

// SSprite
// Simple- or String-Sprite. Sprite for spinners, and such.
// A collecttion of char *frames, with a fix height of 1 line
// (2 blocks).
class SSprite {
public:
  SSprite();
  SSprite(char *str);                          // 1 frame, framecolor=white
  SSprite(char **str, int len);                // array of strs
  SSprite(char **str, int len, RGBColor_t c);   // array of strs, basecolor
  SSprite(char **str, int len, RGBPalette_t p); // array of strs,
                                               // individual colors
  ~SSprite();

  void SetText(char *t);
  void Print();
  void Print(int X, int Y);
  void PrintUncolored();
  void PrintDimmed(); // printf s

  void PrintFrame(int n); // printf a frame
  //
  void SetColor(RGBColor_t c);

  virtual void Render(); //

  int x = 0;
  int y = 0; // in blocks / "half characters"
  int z = 0;
  char *s = 0; // for fast Print() / printf()

  int frame_count = 0;
  SSpriteFrame_t **frames = 0;
  int frame_idx = 0; // current frame

  int counter1 = 0; // convenience counters and thresholds
  int counter2 = 0;
  int counter3 = 0;
  int threshhold1 = 0;
  int threshhold2 = 0;
  int threshhold3 = 0;

  RGBColor_t *background = 0; // for rendering
  int state = 0;             // generic type to support own concepts
private:
  void free_frames();
  unsigned char color_override = 0; // if frame color is unsed,
                                    // or overriden by effect.
};

#endif

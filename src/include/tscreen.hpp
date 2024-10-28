// tscreen.hpp - libtsprites, 2023-24 M64

#ifndef TSL_SCREEN_H
#define TSL_SCREEN_H

#include "tsprites.hpp"
#include "tsrender.hpp"

typedef enum {
    SCREEN_TRANSPARENT,
    SCREEN_BGCOLOR
} TSSCREEN_MODE;

class TScreen {
public:
  TScreen();
  TScreen(int width, int height);
  TScreen(int width, int height, RGBColor_t bgcolor);

  ~TScreen();

  int Height() const;
  int Width() const;

  int X() const;
  int Y() const;
  void SetXY(int px, int py);

  void Clear() const;
  void CClear();

  void AddSprite(TSprite *s);
  void AddSprite(SSprite *s);
  void AddSubScreen(TScreen *s);

  void SetScreenMode(TSSCREEN_MODE m);
  void SetRenderEngine(TSRenderEngineTopDown *engine);

  void Update();
  void Render();

  RGBColor_t bg_color = {0x20, 0x20, 0x20};

  RenderSurface_t *out_surface = 0; // last render of screen, direct access for speed
  char *out_s = 0;
  int is_subscreen = 0;

  TSprite **t_sprites = 0;
  int num_tsprites = 0;
  SSprite **s_sprites = 0;
  int num_ssprites = 0;
  TScreen **sub_screens = 0;
  int num_screens = 0;
    
private:
  void add_out_surface(RenderSurface_t *rs);

  int w = 0;
  int h = 0;

  int x = 0; // position in term, or on screen below
  int y = 0;

  char *clr_line = 0;
  char *scrn_str = 0;
  char *bg_str = 0;


  RenderSurface_t **surfaces_out = 0;
  int num_surfaces_out = 0;
  TSSCREEN_MODE screen_mode = SCREEN_TRANSPARENT;

  TSRenderEngineTopDown *e = 0;
};

#endif

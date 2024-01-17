// tscreen.hpp - libtsprites, 2023-24 M64

#ifndef TSL_SCREEN_H
#define TSL_SCREEN_H

#include "tsrender.hpp"
#include "tsprites.hpp"

class TScreen {
public:
  TScreen();
  TScreen(int width, int height);
  TScreen(int width, int height, rgb_color bgcolor);

  ~TScreen();

  int Height() const;
  int Width() const;

  int X() const;
  int Y() const;

  void SetParent(TScreen *s);
  void SetParent(TScreen *s, int px, int py);
  void SetPos(int px, int py);

  void AddSprite(TSprite *s);
  void AddSprite(SSprite *s);

  void Clear() const;
  void CClear();

  void Update();
  void Render();

  rgb_color bg_color = {0x20, 0x20, 0x20};

private:
  int w = 0;
  int h = 0;

  int x = 0; // position in term, or on screen below
  int y = 0;

  TScreen *parent = 0;

  render_surface *r = 0;

  char *clr_line = 0;
  char *scrn_str = 0;
  char *bg_str = 0;

  TSprite **t_sprites = 0;
  SSprite **s_sprites = 0;
};

#endif

#ifndef TSL_SCREEN_H
#define TSL_SCREEN_H

#include "tscolors.hpp"
#include "tsrender.hpp"

class TScreen {
public:
  TScreen();
  TScreen(int width, int height);

  ~TScreen();

  int Height() const;
  int Width() const;

  void Clear() const;
  void CClear();

  char *scrn_str = 0;
  char *bg_str = 0;

  rgb_color bg_color = {0x20, 0x20, 0x20};

private:
  int w = 0;
  int h = 0;

  render_surface *r = 0;
  char *clr_line = 0;
};

#endif

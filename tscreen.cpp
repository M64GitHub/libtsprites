#include "include/tscreen.hpp"
#include "include/tsutils.hpp"
#include <stdio.h>
#include <stdlib.h>

TScreen::TScreen() {}

TScreen::TScreen(int width, int height) {
  w = width;
  h = height;

  term_init();
  CClear(); // so cursor pos is below screen
}

TScreen::~TScreen() { term_close(); }

int TScreen::Height() const { return h; }

int TScreen::Width() const { return w; };

int TScreen::Y() const { return y; }

void TScreen::SetParent(TScreen *s) { parent = s; }

void TScreen::SetParent(TScreen *s, int px, int py) {
  parent = s;
  x = px;
  y = py;
}

void TScreen::SetPos(int px, int py) {
  x = px;
  y = py;
}

void TScreen::AddSprite(TSprite *s) {}
void TScreen::AddSprite(SSprite *s) {}

void TScreen::Clear() const {
  printf("\x1b[2J"); // erase entire screen, terminal
}

void TScreen::CClear() {
  if (!clr_line) {
    clr_line = (char *)malloc((w + 1) + 2); // ...0x0a0x00
    for (int i = 0; i < w; i++)
      clr_line[i] = ' ';
    clr_line[w] = 0x0a;
  }

  cursor_home();
  set_color(bg_color);
  set_bgcolor(bg_color);
  for (int i = 0; i < h; i++)
    printf("%s", clr_line);

  printf("\x1b[0m"); // reset all modes
  fflush(stdout);

  if (!r)
    return;

  // clear render_surface
  for (int i = 0; i < w * h; i++) {
    r->colormap[i] = bg_color;
    r->shadowmap[i] = 0;
  }
}

// tscreen.cpp - libtsprites, 2023-24 M64

#include "include/tscreen.hpp"
#include "include/tsrender.hpp"
#include "include/tsutils.hpp"
#include <stdio.h>
#include <stdlib.h>

TScreen::TScreen() {}

TScreen::TScreen(int width, int height) {
  out_surface = 0;
  w = width;
  h = height * 2;

  term_init();
  out_surface = new render_surface;

  init_surface(out_surface, w, h, bg_color);
  CClear(); // so cursor pos is below screen
}

TScreen::~TScreen() { term_close(); }

int TScreen::Height() const { return h; }

int TScreen::Width() const { return w; };

int TScreen::Y() const { return y; }

void TScreen::SetPos(int px, int py) {
  x = px;
  y = py;
}

void TScreen::Clear() const {
  printf("\x1b[2J"); // erase entire screen, terminal
}

void TScreen::CClear() {
  // if not yet done: prepare a line full of spaces followed by "\n"
  // it will be printed h/2 times, to clear screen line by line
  // -> TODO: make relative line ends! (for multiple screens)
  if (!clr_line) {
    clr_line = (char *)malloc((w) + 2); // ...0x0a0x00
    for (int i = 0; i < w; i++)
      clr_line[i] = ' ';
    clr_line[w] = 0x0a;
    clr_line[w + 1] = 0x00;
  }

  cursor_home();
  set_color(bg_color);
  set_bgcolor(bg_color);
  for (int i = 0; i < h / 2; i++)
    printf("%s", clr_line);

  printf("\x1b[0m"); // reset all modes
  fflush(stdout);

  if (!out_surface) {
    return;
  }

  // clear render_surface, directly, for speed
  for (int i = 0; i < w * h; i++) {
    out_surface->colormap[i] = bg_color;
    out_surface->shadowmap[i] = 0;
  }
}

// for now: add the sprite's out_surface to the array surfaces_out,
// (for input to the rendering engine) - maybe we need the list of
// attached sprites at some time later
void TScreen::AddSprite(TSprite *spr) {
  TSprite **new_arr = new TSprite *[num_tsprites + 1];

  // copy current
  for (int i = 0; i < num_tsprites; i++) {
    new_arr[i] = t_sprites[i];
  }
  num_tsprites++;

  new_arr[num_tsprites - 1] = spr; // new last one

  delete[] t_sprites;
  t_sprites = new_arr;

  add_out_surface(spr->out_surface); // or do it on each render, so
  // sprite can change it's width/height between renders, too.
  // Then in Render(), the Screen would have to recreate the list of
  // out_surfaces everytime, we don't do that for now, for speed.
}

void TScreen::AddSprite(SSprite *spr) {
  SSprite **new_arr = new SSprite *[num_ssprites + 1];

  for (int i = 0; i < (num_ssprites); i++) {
    new_arr[i] = s_sprites[i];
  }
  num_ssprites++;

  new_arr[num_ssprites - 1] = spr; // new last one

  delete[] s_sprites;
  s_sprites = new_arr;

  // Add surface out
}

// called on each sprite-> add,
// surfaces out holds input array for rendering engine
void TScreen::add_out_surface(render_surface *rs) {
  if (!rs)
    return;

  render_surface **new_arr = new render_surface *[num_surfaces_out + 1];
  // copy current
  for (int i = 0; i < (num_surfaces_out); i++) {
    new_arr[i] = surfaces_out[i];
  }
  num_surfaces_out++;
  new_arr[num_surfaces_out - 1] = rs; // new last one

  delete[] surfaces_out;
  surfaces_out = new_arr;
}

void TScreen::Render() {
  if (!num_surfaces_out)
    return;

  clear_surface_transparent(out_surface);

  // render out_surface
  e->Render(surfaces_out, num_surfaces_out, out_surface);

  // -- OUTPUT --
  cursor_home();
  if(x > 0) cursor_right(x);
  if((y/2) >= 1) cursor_down(y/2);

  // -- create output string
  //
  char buf1k[1024];
  int tmpstr_idx = 0;
  int i = 0;

  rgb_color upper;
  rgb_color lower;

  if (!out_s)
    out_s = new char[w * h * 20 + h + 1024];

  out_s[0] = 0x00; // terminator

  for (int Y = 0; Y < (h); Y += 2) {
    // -- all else lines
    for (int X = 0; X < out_surface->w; X++) {
      upper = out_surface->colormap[X + (Y + 0) * out_surface->w];
      lower = out_surface->colormap[X + (Y + 1) * out_surface->w];

      // upper transp.
      if (!out_surface->shadowmap[X + (Y + 0) * out_surface->w]) {
        if (!out_surface->shadowmap[X + (Y + 1) * out_surface->w]) // and lower
          sprintf(buf1k, "\e[m ");
        else
          sprintf(buf1k, "\x1b[0;38;2;%d;%d;%dm\u2584", // only upper
                  lower.r, lower.g, lower.b);
      } else {
        if (!out_surface
                 ->shadowmap[X + (Y + 1) * out_surface->w]) // lower transparent
          sprintf(buf1k, "\x1b[0;38;2;%d;%d;%dm\u2580", upper.r, upper.g,
                  upper.b);
        else
          sprintf(buf1k,
                  "\x1b[48;2;%d;%d;%dm\x1b[38;2;%d;%d;%dm\u2584", // none
                  upper.r, upper.g, upper.b, lower.r, lower.g, lower.b);
      }
      i = 0;
      while (buf1k[i])
        out_s[tmpstr_idx++] = buf1k[i++];
    }
    // relative line end
    i = 0;
    sprintf(buf1k, "\x1b[%dD", out_surface->w); // cursor go left(lpos)
    while (buf1k[i])
      out_s[tmpstr_idx++] = buf1k[i++];

    i = 0;
    sprintf(buf1k, "\x1b[%dB", 1); // cursor go down(1)
    while (buf1k[i])
      out_s[tmpstr_idx++] = buf1k[i++];
    out_s[tmpstr_idx] = 0x00; // terminator
  }

  printf("%s", out_s);
}

void TScreen::SetRenderEngine(TSRenderEngineTopDown *engine) { e = engine; }

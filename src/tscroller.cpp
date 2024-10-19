// tscroller.cpp - libtsprites, 2023-24 M64

#include "include/tscroller.hpp"
#include "include/tscolors.hpp"
#include "include/tsprites.hpp"
#include "include/tsrendersurface.hpp"

TScroller::TScroller() {}

TScroller::TScroller(TSprite *cs, int slicewidth)
{
  SetCharset(cs, slicewidth);
}

TScroller::TScroller(char *png_filename, int slicewidth)
{
  charset = new TSprite;
  charset->ImportFromPNGFile(png_filename);
  charset->VSplitFixedW(charset->fs.frames[0], slicewidth);
}

TScroller::~TScroller() {}

void TScroller::SetCharset(TSprite *cs, int slicewidth)
{
  charset = cs;
  charset->VSplitFixedW(charset->fs.frames[0], slicewidth);
}

render_surface *TScroller::Render()
{
  return out_surface;
}

void TScroller::SetWH(int W, int H)
{
  rgb_color c = { 0,0,0 };
  w = W;
  h = H;
  out_surface = new render_surface;
  init_surface(out_surface, w, h, c);
}

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

RenderSurface_t *TScroller::Render()
{
  return out_surface;
}

void TScroller::SetWH(int W, int H)
{
  RGBColor_t c = { 0,0,0 };
  w = W;
  h = H;
  out_surface = new RenderSurface_t;
  render_surface_init(out_surface, w, h, c);
}

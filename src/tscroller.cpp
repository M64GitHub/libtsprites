// tscroller.cpp - libtsprites, 2023-24 M64

#include "include/tscroller.hpp"
#include "include/tscolors.hpp"
#include "include/tsprites.hpp"
#include "include/tsrendersurface.hpp"
#include "include/tsutils.hpp"

TScroller::TScroller() {
  for (int i = 0; i < 256; i++)
    translation[i] = 0xFF;
}

TScroller::TScroller(char *png_filename, int slicewidth, char *tr) {
  SetCharset(png_filename, slicewidth);
  SetTranslation(tr);
}

TScroller::~TScroller() {}

void TScroller::SetTranslation(char *tr) {

  for (int i = 0; i < 256; i++)
    translation[i] = 0xFF;

  int slen = strsize(tr) - 1;
  unsigned char c;
  for (int i = 0; i < slen; i++) {
    c = tr[i];
    translation[c] = i + 1; // Frame 0 is whole png
  }
}

void TScroller::SetCharset(char *png_filename, int slicewidth) {
  charset = new TSprite;
  charset->ImportFromPNGFile(png_filename);
  charset->VSplitFixedW(charset->fs.frames[0], slicewidth);
}

RenderSurface_t *TScroller::Render() {
  // return out_surface;
  return full_surface;
}

void TScroller::SetWH(int W, int H) {
  RGBColor_t c = {0, 0, 0};
  w = W;
  h = H;
  out_surface = new RenderSurface_t;
  render_surface_init(out_surface, w, h, c);
}

void TScroller::SetXY(int xx, int yy) {
  x = xx;
  y = yy;
  if (out_surface) {
    out_surface->x = xx;
    out_surface->y = yy;
  }
  if (full_surface) {
    full_surface->x = xx;
    full_surface->y = yy;
  }
}

void TScroller::SetText(char *s) {
  scrolltext = s;
  int slen = strsize(s) - 1;
  int f_idx;

  full_width = 0;
  for (int i = 0; i < slen; i++) {
    f_idx = translation[s[i]];
    if (f_idx == 0xFF)
      continue;
    full_width += charset->fs.frames[f_idx]->w;
  }
  if (full_surface)
    render_surface_free(full_surface);
  if (restore_full_surface)
    render_surface_free(restore_full_surface);

  RGBColor_t c;
  full_surface = new RenderSurface_t;
  restore_full_surface = new RenderSurface_t;
  // height of first char (assuming, all chars the same height
  int chr_height = charset->fs.frames[1]->h;
  render_surface_init(full_surface, full_width, chr_height, c);
  render_surface_init(restore_full_surface, full_width, chr_height, c);

  // render each char onto full_surface
  TSpriteFrame_t *F;
  int chr_width;
  int fulls_x = 0;
  int fulls_y = 0;
  for (int i = 0; i < slen; i++) {
    f_idx = translation[scrolltext[i]];
    if (f_idx == 0xFF)
      continue;
    F = charset->fs.frames[f_idx];
    chr_width = F->w;
    for (int y = 0; y < chr_height; y++) {
      for (int x = 0; x < chr_width; x++) {
        full_surface->colormap[(fulls_y + y) * full_surface->w + fulls_x + x] =
            F->colormap[y * F->w + x];
        full_surface->shadowmap[(fulls_y + y) * full_surface->w + fulls_x + x] =
            F->shadowmap[y * F->w + x];
      } // x
    } // y
    fulls_x += F->w;
  }

  render_surface_copy(full_surface, restore_full_surface);
}

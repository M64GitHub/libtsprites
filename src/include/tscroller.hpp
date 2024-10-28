// tscroller.hpp - libtsprites, 2023-24 M64

#ifndef TSL_TSCROLLER_H
#define TSL_TSCROLLER_H

#include "tsprites.hpp"

class TScroller
{
public:
  TScroller();
  TScroller(TSprite *cs, int slicewidth); 
  TScroller(char *png_filename, int slicewidth); 
  
  ~TScroller();

  void SetText(char *s);
  void SetCharset(TSprite *cs, int slicewidth);
  void SetCharset(TSprite *cs, int *widths, int num_slices);
  void SetCharset(char *png_filename, int slicewidth);
  void SetCharset(char *png_filename, int *slicewidths, int num_slices);
  void SetWH(int w, int h);

  void Tick();

  RenderSurface_t *Render();

private:
  int x;
  int y;

  int h;
  int w;

  char *scrolltext;
  TSprite *charset;

  RenderSurface_t *out_surface = 0;
};


#endif

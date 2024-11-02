// tscroller.hpp - libtsprites, 2023-24 M64

#ifndef TSL_TSCROLLER_H
#define TSL_TSCROLLER_H

#include "tsprites.hpp"
#include "tsrendersurface.hpp"

class TScroller
{
public:
  TScroller();
  TScroller(char *png_filename, int slicewidth, char *tr); 
  
  ~TScroller();

  void SetText(char *s);
  void SetCharset(char *png_filename, int slicewidth);
  // void SetCharset(char *png_filename, int *slicewidths, int num_slices);
  void SetWH(int w, int h);
  void SetXY(int x, int y);
  void SetTranslation(char *tr);

  void Tick();

  RenderSurface_t *Render();

  RenderSurface_t *out_surface = 0;
  RenderSurface_t *restore_surface = 0;
  RenderSurface_t *full_surface = 0;
  RenderSurface_t *restore_full_surface = 0;
  int x;
  int y;
  int full_width;

private:
  int h;
  int w;

  char *scrolltext;
  
  TSprite *charset;
  char translation[256];
};


#endif

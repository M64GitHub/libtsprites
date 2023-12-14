#ifndef TSPRITES_H
#define TSPRITES_H

#define DEBUG

#ifdef DEBUG
#define DBG(...) fprintf (stderr, __VA_ARGS__)
#else 
#define DBG(...) 
#endif


class TSprite
{
public:
    TSprite();
    TSprite(int ww, int hh);
    TSprite(char *imgstr);

    ~TSprite();

    int ImportFromImgStr(char *s);
    int ImportFromFile(char *fn);
    int ImportFromColorMap(char *cmap);

    int ExportImgStr(char *fn, int format); // export as .h, txtfile, ...

    void Reset(); // restores original state (after 
                  // possible applied effects)

    void Print(); // output s

    // main attributes w/o getters for fastest access
    int w = 0;
    int h = 0;
    char *s = 0; // for fast printf(), restored after Reset()

    // get/set easily, directly
    int x = 0;
    int y = 0;
    int z = 0;

    // internal representation - for fast effects processing, ...
    char *shadow_map; // w * h: like alpha channel: 0: no px, 1: px
                      // for collision detection, Print() ...
    char *color_map;  // w * h: r, g, b

private:
    int malloc_maps();
    int free_maps();

    int maps_initialized = 0; // either all or none,
                              // for better readibility          

    // copies of initial state
    char *s_source;           // internal, converted representation 
    int   s_source_len;       // for speed, to avoid strlen()
    char *shadow_map_source;  //
    char *color_map_source;   //


};

#endif


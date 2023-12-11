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

    void Reset(); // restores original state (after 
                  // possible applied effects

    void Print();

    // main attributes w/o getters for fastest access
    int w = 0;
    int h = 0;
    char *s = 0; // for fast access to printf() it 

    int x = 0;
    int y = 0;
    int z = 0;

    char *shadow_map; // w * h
    char *color_map;  // w * h: r, g, b
    char *lines[128]; // for fast access to y-stretch effects,
                      // more than enough. maybe remove at all

private:
    int malloc_maps();
    int free_maps();

    // copies of initial state
    char *s_source;
    char *shadow_map_source;
    char *color_map_source;
    int maps_initialized = 0; // either all or none,
                              // for better readibility          
};

#endif


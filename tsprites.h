#ifndef TSPRITES_H
#define TSPRITES_H

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

    void Print();

    // main attributes w/o getters for fastest access
    int w = 0;
    int h = 0;
    char *s = 0; // for fastest access to printf() it 

    int x = 0;
    int y = 0;
    int z = 0;

    char *shadow_map; // w * h
    char *color_map;
    char *lines[128]; // more than enough

private:
    int malloc_maps();
    int free_maps();
    
};

#endif


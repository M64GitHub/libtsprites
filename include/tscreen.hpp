#ifndef TS_SCREEN_H
#define TS_SCREEN_H

#include "tscolors.hpp"

class TScreen {
public:
    TScreen();
    TScreen(int width, int height);

    ~TScreen();

    rgb_color *colormap   = 0;
    rgb_color *background = 0;

    int Height() const;
    int Width()  const;

    void Clear() const;
    void CClear(char c);

    char *scrn_str = 0;
    char *bg_str   = 0;

    rgb_color fg_color = { 0x40, 0x40, 0x40 };
    rgb_color bg_color = { 0x20, 0x20, 0x20 };

private:
    int w = 0;
    int h = 0;

    char *clr_line = 0;
};

#endif


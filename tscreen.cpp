#include "include/tscreen.hpp"
#include "include/tsutils.hpp"
#include <stdio.h>
#include <stdlib.h>

TScreen::TScreen() 
{
}

TScreen::TScreen(int width, int height) 
{
    w = width;
    h = height;

    term_init();
    CClear(' '); // so cursor pos is below screen
}

TScreen::~TScreen()
{
    term_close();
}

int TScreen::Height() const
{
    return h;
}
   
int TScreen::Width() const
{
    return w;
}

void TScreen::Clear() const
{
    printf("\x1b[2J");   // erase entire screen, terminal
                         // fastest
}

void TScreen::CClear(char c) 
{
    if(!clr_line)
        clr_line = (char *) malloc((w + 1) + 2); // ...0x0a0x00
    
    if(clr_line[0] != c) {
        for(int i=0; i < w; i++) clr_line[i] = c;
        clr_line[w] = 0x0a;
    }
    cursor_home();
    set_color(fg_color);
    set_bgcolor(bg_color);
    for(int i=0;i<h;i++) printf("%s", clr_line);
    printf ("\x1b[0m");   // reset all modes
    fflush(stdout);
}


#include <math.h>   // for sin()
#include <unistd.h> // for usleep()
#include "include/tsprites.hpp"

#include <stdio.h>

int main(int argc, char **argv)
{
    TSprite S;

    // import sprite graphics 
    S.ImportFromFile((char*)"resources/demo7t_188.unicode");

    // add (append) another frame 
    S.ImportFromFile((char*)"resources/demo6_180.unicode");

    S.Print();  

    cursor_right(10);
    S.PrintFrame(0);
    cursor_left(5);
    S.PrintFrame(1);
    cursor_left(5);

    S.PrintDebugMap(S.frames[0]);
    S.PrintDebugMap(S.frames[1]);

    return 0;

    // --
}

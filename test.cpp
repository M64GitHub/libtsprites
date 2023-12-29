#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "tsprites.hpp"

int main(int argc, char **argv)
{
    TSprite S;
    unsigned int tick = 50;

    printf("catimg to tsprites conversion/import test utility.\n");
    printf("M64, 2023.\n");

    S.ImportFromFile((char*)"resources/demo6_180.unicode");
    cursor_down(S.h/2+5);
    cursor_up(S.h/2+5);

    cursor_off();

    while(1)
    {
        cursor_reset();  // reset all modes and colors
        tick++;
        int x = 10 + 10*(sin( ((tick % 100)/100.0) * 6.28  ));
        cursor_right(x);
        S.Print();
        cursor_up(S.h/2 + 2);
        usleep(1000 * 20);
    }

    cursor_on();
    
    return 0;
}

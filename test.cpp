#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "tsprites.hpp"

int main(int argc, char **argv)
{
    TSprite S;
    unsigned int tick = 0;

    printf("catimg to tsprites conversion/import test utility.\n");
    printf("M64, 2023.\n");

    S.ImportFromFile((char*)"resources/demo6_180.unicode");
    cursor_down(S.h+5);
    cursor_up(S.h+5);

    while(1)
    for(int i=0; i<10; ++i) {
        printf ("\x1b[0m\n");   // reset all modes
        tick++;
        int x = 10 + 10*(sin( ((tick % 100)/100.0) * 6.28  ));
        cursor_right(x);
        S.Print();
        cursor_up(S.h + 2);
        usleep(5000 * 2);
    }
    
    return 0;
}

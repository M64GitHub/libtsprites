#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "include/tsprites.hpp"

int main(int argc, char **argv)
{
    TSprite S;
    SSprite S2((char *)"_.:[M64]:._");

    unsigned int tick = 50;
    unsigned int maxtick = 850;

    S.ImportFromFile((char*)"resources/demo6_180.unicode");

    board_init();

    while(tick < maxtick)
    {
        tick++;
        int x = 10 + 10*(sin( ((tick % 100)/100.0) * 6.28  ));
        cursor_home();
        cursor_right(x-1);
        S.Print();
        cursor_up(1);
        cursor_right(90);
        cursor_right((x-1)/4);
        S2.PrintUncolored();
        fflush(stdout);
        usleep(1000 * 10);
    }
    
    board_close();

    return 0;
}

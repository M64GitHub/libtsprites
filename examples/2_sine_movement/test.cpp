#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "../../tsprites.hpp"

int main(int argc, char **argv)
{
    TSprite S;
    unsigned int tick = 50;
    unsigned int maxtick = 250;

    printf("catimg to tsprites conversion/import test utility.\n");
    printf("M64, 2023.\n");

    S.ImportFromFile((char*)"../../resources/demo6_180.unicode");

    // save and clear terminal, set cursor to home position (0,0) and hide it
    board_init(); 

    while(tick < maxtick)
    {
        tick++;
        int x = 10 + 10*(sin( ((tick % 100)/100.0) * 6.28  ));
        cursor_home();
        cursor_right(x);
        S.Print();
        usleep(1000 * 10);
    }
    
    board_close(); // restore screen and cursor
    return 0; 
}

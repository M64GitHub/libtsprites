#include <math.h>   // for sin()
#include <unistd.h> // for usleep()
#include <stdio.h>  // for printf()
#include "../../include/tsprites.hpp"
#include "../../include/tsutils.hpp"

int main(int argc, char **argv)
{
    char *spinners[] = { 
                        (char*) " - ▁   SSprite ", 
                        (char*) " \\ ▂ ░ SSprite ",
                        (char*) " | ▃ ▒ SSprite ",
                        (char*) " / ▄ ▓ SSprite ",
                        (char*) " - ▅ █ SSprite ",
                        (char*) " \\ ▆ ▓ SSprite ",
                        (char*) " | ▇ ▒ SSprite ",
                        (char*) " / █ ░ SSprite ",
    };

    rgb_color spinners_color = { 0x9C, 0x41, 0xdE }; // R, G, B

    TSprite TS;

    SSprite S1((char *)"_.:[M64]:._");
    SSprite S2(spinners, 8, spinners_color);

    TS.counter1    =  50;
    TS.threshhold1 = 850;

    TS.ImportFromFile((char*)"../../resources/demo7_188.unicode");

    term_init();

    while(TS.counter1 < TS.threshhold1)
    {
        TS.counter1++;
        int x = 10 + 10*(sin( ((TS.counter1 % 100)/100.0)*6.28 ))-1;
        cursor_home();
        cursor_right(x);
        TS.Print(); 
        printf("\n");

        cursor_right(90 + x/4);
        S1.PrintUncolored();

        // animate spinners by selecting frame to be printed
        cursor_left (90 + x/4);
        if(!(TS.counter1 % 8)) S2.frame_idx = (S2.counter1++ % 8);
        S2.Print();

        usleep(1000 * 10);
    }
    
    term_close();

    return 0;
}

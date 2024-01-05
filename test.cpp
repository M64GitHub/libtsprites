#include <math.h>   // for sin()
#include <unistd.h> // for usleep()
#include "include/tsprites.hpp"

#include <stdio.h>

int main(int argc, char **argv)
{
    char *spinner[] = { 
                        (char*) " - ▁   SSprite ", 
                        (char*) " \\ ▂ ░ SSprite ",
                        (char*) " | ▃ ▒ SSprite ",
                        (char*) " / ▄ ▓ SSprite ",
                        (char*) " - ▅ █ SSprite ",
                        (char*) " \\ ▆ ▓ SSprite ",
                        (char*) " | ▇ ▒ SSprite ",
                        (char*) " / █ ░ SSprite ",
    };

    rgb_color spinner_color = { 0x9C, 0x41, 0xdE }; // R, G, B

    TSprite S;
    SSprite S2((char *)"_.:[M64]:._");
    SSprite S3(spinner, 8, spinner_color);

    unsigned int tick = 50;
    unsigned int maxtick = 850;
    int spinner_tick = 0;

    S.ImportFromFile((char*)"resources/demo7t_188.unicode");

    // -- 
    S.ImportFromFile((char*)"resources/demo6_180.unicode");

    S.Print();
    cursor_right(10);
    printf("%s", S.frames[0]->s);
    cursor_left(5);
    printf("%s", S.frames[1]->s);
    cursor_left(5);

    S.PrintDebugMap(S.frames[0]);
    S.PrintDebugMap(S.frames[1]);

    return 0;

    // --



    screen_init();

    // --

    while(tick < maxtick)
    {
        tick++;
        int x = 10 + 10*(sin( ((tick % 100)/100.0) * 6.28  ));
        cursor_home();
        cursor_right(x-1);
        S.Print();

        cursor_right(90);
        cursor_right((x-1)/4);
        S2.PrintUncolored();

        // spinner
        cursor_home();
        cursor_down(15);
        cursor_right(5);
        if(!(tick % 8)) S3.frame_idx = (++spinner_tick % 8);
        S3.Print();

        usleep(1000 * 10);
    }

    // --
    
    screen_close();

    return 0;
}

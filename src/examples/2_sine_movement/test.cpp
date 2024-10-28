#include <math.h>   // for sin()
#include <unistd.h> // for usleep()
#include <stdio.h>  // for printf()
#include "../../include/tsprites.hpp"
#include "../../include/tsutils.hpp"

int main(int argc, char **argv)
{
    char *spinners[] = { // array of strings for the spinner sprite below
                        (char*) " - ▁   SSprite ", 
                        (char*) " \\ ▂ ░ SSprite ",
                        (char*) " | ▃ ▒ SSprite ",
                        (char*) " / ▄ ▓ SSprite ",
                        (char*) " - ▅ █ SSprite ",
                        (char*) " \\ ▆ ▓ SSprite ",
                        (char*) " | ▇ ▒ SSprite ",
                        (char*) " / █ ░ SSprite ",
    };

    RGBColor_t spinners_color = { 0x9C, 0x41, 0xdE }; // R, G, B

    TSprite TS; // create TSprite and load the graphics data
    TS.ImportFromFile((char*)"../resources/demo7t_188.unicode");

    SSprite S1( (char *)"_.:[M64]:._" );       // create a string-sprite
    SSprite S2( spinners, 8, spinners_color ); // create a string sprite,
                                               // add 8 frames "animation"

    TS.counter1    =  50; // start value for x-movement while loop
    TS.threshhold1 = 850; // end   value for x-movement while loop

    term_init();          // clear terminal

    while(TS.counter1 < TS.threshhold1) 
    {
        TS.counter1++;
        cursor_home();    // move the cursor to topleft in terminal
        int x = 10 + 10*(sin( ((TS.counter1 % 100)/100.0)*6.28 ))-1;

        cursor_right(x);  // position TS
        TS.Print();       // and print it
        printf("\n");

        cursor_right(90 + x/4); // position S1
        S1.PrintUncolored(); // prints in default terminal color

        // animate spinners by selecting frame to be printed: S2.frame_idx
        cursor_left (90 + x/4); // position S2
        if(!(TS.counter1 % 8)) S2.frame_idx = (S2.counter1++ % 8);
        S2.Print();             // print it

        usleep(1000 * 10);// wait 10 milliseconds
    }
    
    term_close();         // restore terminal 
    return 0;
}

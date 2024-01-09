// Moving spaceship example                                         2024, M64
#include <math.h>   // for sin()
#include <unistd.h> // for usleep()
#include "include/tscreen.hpp"
#include "include/tsprites.hpp"
#include "include/tsutils.hpp"

#include <stdio.h>

int main(int argc, char **argv)
{
    TScreen Screen(120, 45);
    TSprite S1;
    TSprite Spaceship;

    ruler(120);

    S1.ImportFromFile(
       (char *)"resources/demo7t_188.unicode");
    
    // --
    Spaceship.ImportFromFile(
        (char *)"resources/spaceship1_1.unicode");

    Spaceship.PrintDebugMap(Spaceship.frames[0]);

    S1.Print(20, 0);

    Spaceship.Print(20, 30);
    Spaceship.Print(40, 31);

    // --

    while(1) {
        usleep(1000 * 20);
    }

    return 0;
}

#include <math.h>   // for sin()
#include <unistd.h> // for usleep()
#include "include/tscreen.hpp"
#include "include/tsprites.hpp"
#include "include/tsutils.hpp"

#include <stdio.h>

int main(int argc, char **argv)
{
    TScreen Screen(120, 30);
    // TSprite S1;
    TSprite Spaceship;

    ruler(120);

    // S1.ImportFromFile(
    //     (char *)"resources/demo7t_188.unicode");

    //
    // --
    Spaceship.ImportFromFile(
        (char *)"resources/spaceship1_1.unicode");

    Spaceship.PrintDebugMap(Spaceship.frames[0]);

    // S1.Print(20, 0);
    Spaceship.Print(0, 0);
    Spaceship.Print(20, 4);
    Spaceship.Print(36, 5);

    // --
    
    char tmp[16]; int n = 0;
    n = fread(tmp, 1, 1, stdin);

    return 0;
}

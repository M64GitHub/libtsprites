#include <stdio.h>
#include "../../include/tsprites.hpp"
#include "../../include/tsutils.hpp"

int main(int argc, char **argv)
{
    TSprite S;                    // create a TSprite object S

    // check cmdline for filename
    if (argc != 2) { printf("Usage: ./test <filename>\n"); return 1; }

    char *file_name = argv[1];    // get filename from cmdline

    // term_clear();                 // clear the screen
    printf("hello world!\n");
    S.ImportFromPNGFile(file_name);

    // NOTE: enable line below to print the visual map representation
    // S.PrintDebugMap(S.fs.frames[0]);

    // we need this, in case we stand at the bottom of the terminal,
    // in order to have space to print the sprite
    cursor_down(S.h/2);
    cursor_up(S.h/2);

    S.Print();

    return 0;
}

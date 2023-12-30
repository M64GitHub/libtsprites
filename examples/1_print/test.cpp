#include <stdio.h>
#include "../../tsprites.hpp"

int main(int argc, char **argv)
{
    char *ifile_name = 0;

    // create a TSprite object S
    TSprite S;

    // check cmdline for filename
    if (argc != 2) { printf("Usage: ./test <filename>\n"); return 1; }

    ifile_name = argv[1];         // get filename from cmdline

    printf("hello world!\n");
    
    S.ImportFromFile(ifile_name); // import catimg redirected output file

    cursor_right(5);              // cursor movement functions are included
    cursor_down(S.h / 2);         // move cursor down to make space for the sprite
    cursor_up(S.h / 2);           // and move cursor back up the height of the sprite

    S.Print(); // print the sprite!

    return 0;
}
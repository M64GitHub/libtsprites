#include <stdio.h>
#include "../../include/tsprites.hpp"
#include "../../include/tsutils.hpp"

int main(int argc, char **argv)
{
    TSprite S;                    // create a TSprite object S

    // check cmdline for filename
    if (argc != 2) { printf("Usage: ./test <filename>\n"); return 1; }

    char *file_name = argv[1];    // get filename from cmdline

    term_clear();                 // clear the screen
    printf("hello world!\n");
    S.ImportFromFile(file_name);  // import catimg redirected output file
    S.Print();                    // print the sprite!

    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "tsprites.hpp"

void usage() 
{
    printf("\nUsage:\n\n");
    printf("test <inputfile>\n");

    exit(1);
}

int main(int argc, char **argv)
{
    char *ifile_name = 0;
    TSprite S;

    printf("catimg to tsprites conversion/import test utility.\n");
    printf("M64, 2023.\n");

    if (argc != 2) usage();

    ifile_name = argv[1];

    printf("[MAIN] working with: '%s' as input file.\n", ifile_name);
    S.ImportFromFile(ifile_name);

    cursor_down(S.h); // make space for the sprite
    cursor_up(S.h);
    cursor_right(5);

    S.Print();
    
    return 0;
}

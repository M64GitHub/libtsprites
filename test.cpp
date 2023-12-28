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

int xpos[] = { 
    0, 1, 2, 
    3, 3, 
    4, 4, 4,
    5, 5, 5, 5, 
    6, 6, 6, 6, 6, 
    7, 7, 7, 7, 7
};

int main(int argc, char **argv)
{
    char *ifile_name = 0;
    TSprite S;

    printf("catimg to tsprites conversion/import utility.\n");
    printf("M64, 2023.\n");

    if (argc != 2) usage();

    ifile_name = argv[1];

    printf("[MAIN] working with: '%s' as input file.\n", ifile_name);
    S.ImportFromFile(ifile_name);

    while(1)
    for(int i=0; i<10; ++i) {
        printf ("\x1b[0m\n"); 
        printf("\x1b[H"); // home
        printf ("\x1b[2J"); // clr term
        for(int j=0; j<i; ++j) {
            printf(" ");
        }
        S.Print();
        printf ("\x1b[0m\n"); 
        usleep(75000);
    }
    return 0;
}

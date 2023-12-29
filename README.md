# libtsprites
C++ library for truecolor unicode terminal sprites, effects, and animations. Blazingly fast! Ideal for terminal games, or just more interesting user interfaces.

Work in progress ...

## Example Code

### Basic Usage

```C++
#include <stdio.h>
#include "tsprites.hpp"

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
    cursor_down(S.h);             // move cursor down to make space for the sprite
    cursor_up(S.h);               // and move cursor back up the height of the sprite

    S.Print(); // print the sprite!

    return 0;
}
```
![image](https://github.com/M64GitHub/libtsprites/assets/84202356/53995d62-ef77-4bd9-be4d-c3d081ebb1f1)

### Sine Movement Test 
```C++
include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "tsprites.hpp"

int main(int argc, char **argv)
{
    TSprite S;
    unsigned int tick = 0;

    printf("catimg to tsprites conversion/import test utility.\n");
    printf("M64, 2023.\n");

    S.ImportFromFile((char *)"resources/demo6_180.unicode");
    cursor_down(S.h+5);
    cursor_up(S.h+5);

    while(1) {
        printf ("\x1b[0m\n");   // reset all modes
        tick++;
        int x = 10 + 10*(sin( ((tick % 100)/100.0) * 6.28  ));
        cursor_right(x);
        S.Print();
        cursor_up(S.h + 2);
        usleep(5000 * 2);
    }
    
    return 0;
}
```


https://github.com/M64GitHub/libtsprites/assets/84202356/049a5c6c-08c5-45a4-8f41-d462da121f5b





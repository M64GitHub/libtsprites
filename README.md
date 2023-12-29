# libtsprites
C++ library for truecolor unicode terminal sprites, effects, and animations. Blazingly fast! Ideal for terminal games, or just more interesting user interfaces.

Work in progress ...
(see tsprites.hpp)

## CONCEPT

The whole lib is based on the concept of `blocks`, not characters. 2 blocks stacked form a regular character. A regular character has the height of 2, an upper block and a lower block. ASCII characters are 2 units in height.

### Main rinciples

 - `blocks`: half characters. Have a color or can be transparent: '▄' or '▀' are individual blocks. 
 - `soft blocks` and `soft lines`: leveraging UTF-8 block characters for pixelwise smooth (constrained) movements and positioning. The UTF-8 block characters enables to form 8x8 blocks '█', that can be moved pixelwise: '▐▌'. This is achieved by using 2 characters and specific combinations of foreground and background colors of the supported block character-set. A soft block is visually always 8x8 pixels in dimension. The same concept also can be used to form lines of arbitrary length in pixels: '▐█▌'.
 - `characters`:  ASCII/UNICODE characters


## EXAMPLE CODE
How to use the the TSprite object.

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
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "tsprites.hpp"

int main(int argc, char **argv)
{
    TSprite S;
    unsigned int tick = 50;

    printf("catimg to tsprites conversion/import test utility.\n");
    printf("M64, 2023.\n");

    S.ImportFromFile((char*)"resources/demo6_180.unicode");
    cursor_down(S.h+5);
    cursor_up(S.h+5);

    cursor_off();

    while(1)
    {
        cursor_reset();  // reset all modes and colors
        tick++;
        int x = 10 + 10*(sin( ((tick % 100)/100.0) * 6.28  ));
        cursor_right(x);
        S.Print();
        cursor_up(S.h + 2);
        usleep(5000 * 2);
    }

    cursor_on();
    
    return 0;
}
```


https://github.com/M64GitHub/libtsprites/assets/84202356/51be2ddf-c30f-4ccd-9e48-b01b8014748a






# libtsprites
C++ library for truecolor unicode terminal sprites, effects, and animations. Blazingly fast! Ideal for terminal games, or just more interesting user interfaces.

Work in progress ...
(see tsprites.hpp)

## CONCEPT

The whole lib is based on the concept of `blocks`, not characters. 2 blocks stacked form a regular character. A regular character has the height of 2, an upper block and a lower block. ASCII characters are 2 units in height. Rectangular shapes formed of `blocks` are called `regular sprites` - the code representation would be a `TSprite`. 

The coordinate-system by default uses measurements in blocks. `TSprites` can be moved freely on a virtual coordinate-system measured in blocks. Basically this means the Y-coordinates have twice the resolution of a character. ASCII characters in strings can be moved in blocksize in the X-dimension, but only in steps of 2 in the Y-dimension for example.   

With a bit of trickery, also completely smooth (almost pixelwise) movements of a pair of blocks can be achieved: in 1/8 fraction steps of a characters dimension. This only works for "logical blocks" in dimensions of a character (2 blocks in height). Those "double blocks" are called `soft blocks`.  
`soft-blocks` can be smoothly moved only: in either the X- or the Y-coordinate, not both at the same time.  

Combining 2 blocks to a soft block and using the same technique - also lines of arbitrary length are possible. Such lines share the movement property of a `soft-block` and are called `soft-lines`. Soft-blocks and soft-lines can be mainly used for a very limited usage-set due to their rather "big" visual blockyness of a full character. But with their interesting possibility to move them "freely" and "smooth", some nice special-effects are possible.  

In this means a special type of sprites is available, too: `line sprites`. Their code representation would be an `LSprite`. Imagine the `M64` logo on this page for example. It can be represented as a `line-sprite` and moved smoothly around the screen.

### Summary of Main Principles

 - `blocks`: half characters. Have a color or can be transparent: '▄' or '▀' are individual blocks. Shapes of blocks are called `regular sprites` or `TSprites`.
 - `soft blocks` and `soft lines`: leveraging UTF-8 block characters for pixelwise smooth (constrained) movements and positioning.  
The UTF-8 block characters enables to form (virtual) 8x8 blocks '█', that can be moved "pixelwise": '▐▌'. This is achieved by using 2 characters and specific combinations of foreground and background colors of the supported block character-set. A soft block is visually always (virtual) 8x8 pixels in dimension.  
The same concept also can be used to form lines of arbitrary length in virtual pixels: '▐█▌', '██▌'. A special type of sprites take leverage from these objects and is called `LSprite` or `line-sprite`.
 - `characters`:  ASCII/UNICODE characters. Shapes out of ASCII characters are called `ascii-sprites` or `ASprite`.


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






# libtsprites
C++ library for truecolor unicode terminal sprites, effects, and animations.
Blazingly fast! Ideal for terminal games, or just more interesting user
interfaces.  

The intention is to create a library for visual expression on state of the art, fast terminals.
A well setup terminal today allows for very interesting visual experiences
and effects. This is an exploration of what can be achieved taking it to
the max.

It is assumed, that the terminal supports: UTF-8 unicode, 24 bit colors, and
is set-up with a proper monospaced font. Tests so far were done on kitty, the
gnome-terminal (Ubuntu default), and iTerm2 on the mac. Those for sure work
well. A note about nerd-fonts (https://www.nerdfonts.com/font-downloads)
shall be given here as well.

Work in progress ...
(see tsprites.hpp)

## CONCEPT

The whole lib is based on the concept of `blocks`, not characters. 2 blocks
stacked form a regular character. A regular character has the height of 2,
an upper block and a lower block. ASCII characters are 2 units in height.
Rectangular shapes formed of `blocks` are called `regular sprites` - the
code representation would be a `TSprite`.

The coordinate-system by default uses measurements in blocks.
`TSprites` can be moved freely on a virtual coordinate-system measured in
blocks. Basically this means the Y-coordinates have twice the resolution of
a character. ASCII characters in strings can be moved in blocksize in the
X-dimension, but only in steps of 2 in the Y-dimension for example.

With a bit of trickery, also completely smooth (almost pixelwise) movements
of a pair of blocks can be achieved: in 1/8 fraction steps of a characters
dimension. This only works for "logical blocks" in dimensions of a character
(2 blocks in height). Those "double blocks" are called `soft blocks`.  
`soft-blocks` can be smoothly moved only: in either the X- or the
Y-coordinate, not both at the same time.

Combining 2 blocks to a soft block and using the same technique - also lines
of arbitrary length are possible. Such lines share the movement property of
a `soft-block` and are called `soft-lines`. Soft-blocks and soft-lines can be
mainly used for a very limited usage-set due to their rather "big" visual
blockyness of a full character. But with their interesting possibility to
move them "freely" and "smooth", some nice special-effects are possible.  

In this means a special type of sprites is available, too: `line sprites`.
Their code representation would be a `LSprite`. Imagine the `M64` logo on
this page for example. It can be represented as a `line-sprite` and moved
smoothly around the screen.

### Summary of Main Principles

 - `blocks`: half characters. Have a color or can be transparent: '▄' or '▀'
 are individual blocks. Shapes of blocks are called `regular sprites` or
 `TSprites`.
 - `soft blocks` and `soft lines`: leveraging UTF-8 block characters for
 pixelwise smooth (constrained) movements and positioning.  
The UTF-8 block characters enables to form (virtual) 8x8 blocks '█', that
can be moved "pixelwise": '▐▌'. This is achieved by using 2 characters and
specific combinations of foreground and background colors of the supported
block character-set. A soft block is visually always (virtual) 8x8 pixels in
 dimension.  
The same concept also can be used to form lines of arbitrary length in
virtual pixels: '▐█▌', '██▌'. A special type of sprites take leverage from
these objects and is called `LSprite` or `line-sprite`.
 - `characters`:  ASCII/UNICODE characters. Shapes out of ASCII characters
are called `ascii-sprites` or `ASprite`.


## EXAMPLE CODE
How to use the the `TSprite` object.

### Basic Usage
In this example, a `TSprite` is created by importing it from the resource 
folder from a file. The filename is read as a commandline parameter.
The method `ImportFromFile()` is used for import, and `Print()` is
used to print it on screen.

Sprites can be created by using the `catimg` utility 
(https://github.com/posva/catimg). Simply redirect it's output to a file.

When a `TSprite` is imported, it is being rewritten into internal datastructures,
and prepared for fast movement on screen / for applying various effects to it.

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
    cursor_down(S.h/2);           // move cursor down to make space for the sprite
    cursor_up  (S.h/2);           // and move cursor back up the height of the sprite

    S.Print(); // print the sprite!

    return 0;
}
```
![image](https://github.com/M64GitHub/libtsprites/assets/84202356/53995d62-ef77-4bd9-be4d-c3d081ebb1f1)

### Sine Movement Test
Here the convenience functions `board_init` and `board_close` are introduced. 
They will clear the terminal and restore the screen and cursor afterwards.
A specific concept of a "board" like a "game board" is not present or planned,
so they do not even take any parameters.

As you can see, the movement of the sprite is done simply by moving the cursor,
and printing the sprite like in the above example. This is one way to easily 
position a sprite. `Print()` just prints the sprite where the cursor currently 
stands.

```C++
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "tsprites.hpp"

int main(int argc, char **argv)
{
    TSprite S;
    unsigned int tick = 50;
    unsigned int maxtick = 250;

    printf("catimg to tsprites conversion/import test utility.\n");
    printf("M64, 2023.\n");

    S.ImportFromFile((char*)"resources/demo6_180.unicode");

    // save and clear terminal, set cursor to home position (0,0) and hide it
    board_init(); 

    while(tick < maxtick)
    {
        tick++;
        int x = 10 + 10*(sin( ((tick % 100)/100.0) * 6.28  ));
        cursor_home();
        cursor_right(x);
        S.Print();
        usleep(1000 * 10);
    }
    
    board_close(); // restore screen and cursor
    return 0; 
}
```



https://github.com/M64GitHub/libtsprites/assets/84202356/4927a436-3f47-4a18-970f-020eb9ee50fe







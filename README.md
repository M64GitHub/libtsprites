# libtsprites
C++ library for truecolor unicode terminal sprites, effects, and animations.
Blazingly fast! Ideal for terminal games, or just more interesting user
interfaces.  

The intention is to create a library for visual expression on state of the art, 
fast terminals.
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

## MAIN CONCEPT

In regard to graphics, the whole lib is based on the concept of `blocks`, 
not characters. 2 blocks stacked form a regular character. A regular character 
has the height of 2, an upper block and a lower block. ASCII characters are 
2 units in height.
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

### Summary of Main Principles, Shapes and Objects

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
these shapes and is called `LSprite` or `line-sprite`.
 - `characters`:  ASCII/UTF-8 characters. Shapes out of UTF-8 characters
can be built and are called `ascii-sprites` or `ASprite`.
 - `strings`: to work with text, another sprite class called `SSprite` 
(`string sprite`) exists. This can be used to place/fade text onto the screen,
create spinners (1 chareacter animations), and such.
 - `frames`: each sprite can hold multiple shapes of itself: for creating 
animations, color-fading, or different rotations of a moving player figure for
example.

The different types of sprites exist to help in the realisation of ideas.
Each sprite type has it's own capabilities, pro's and con's. 

### Classes / Structs

 - TSprite
 - LSprite
 - ASprite
 - SSprite

 - rgb_color
 - rgb_palette

## BUILD
You can build the library along with a simple test program by running
```bash
make
```
The shared library can then be found in the `lib/` folder.
The test executable ('test') will be created in the current path.
You can run it via
```bash
LD_PRELOAD=./lib/libtsprites.so ./test
```
To build the library only:
```bash
make lib
```
For building your own applications using libtsprites 
 - include the`<PATH TO include>/tsprites.hpp` in your code
 - optionally add `-I<PATH TO include>` to your compiler-flags
 - add `-ltsprites` and `-L<PATH TO .so file>` in the linking step

The most easy way to do all this is to copy the library to `/usr/lib/`. 
```bash
sudo cp lib/libtsprites.so /usr/lib
```
You then need not set LD_RELOAD and such for running your program,
and are done by just including the header and adding `-ltsprites`
to your linker flags.

## EXAMPLE CODE
For getting started quickly, example code is provided in the subfolder 
`examples/`. They all have their own Makefile and include the library code
directly, without the need of compiling the library as a shared object.
You can use them as a starter for your own ideas, and copy/extend them
without ever compiling the library, or have the need to link against
it, if you prefer.

### Basic Usage
How to use the the `TSprite` object.

Getting started is simple. The two most basic methods of `Tsprite` are used
in this little "hello world" example: `ImportFromFile()`, and `Print()`.

Sprite graphics can be created with help of the `catimg` utility 
(https://github.com/posva/catimg). Simply redirect it's output to a file.
You can also take any "*.unicode" file from the resource folder to test.

When a `TSprite` is imported, the graphic data is being parsed, normalized,
and rewritten into the internal datastructures. These prepare for fast 
movement on screen, and for applying effects to the graphic data more
efficiently.

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

### Sine Movement
Here the convenience functions `board_init` and `board_close` are introduced. 
They will clear the terminal and restore the screen and cursor afterwards.
(A specific concept of a "board" like a "game board" is not yet implemented,
but planned).

Also two new `SSPrites` ("String Sprites") are introduced: `S2` for some animated 
spinners, and `S3` for a simple (single-frame) string.
The spinner S2 takes an array of strings, it's length (number of sprite frames), and a
`rgb_color` as input parameters for the constructor. The other one just a regular (char *) 
string.

As you can see, the movement of the sprites is done simply by moving the cursor,
and printing the sprites like in the example above. This is one way to easily
position a sprite. `Print()` just prints the sprite where the cursor currently
stands.  
`S2` is printed via `PrintUncolored()`. This is a method specific to string-sprites:
the color information is completely discarded on output, that means the default 
terminal color is used.

Since the Sprites S and S3 are not being "moved", they are also not cleared from the old to 
the new position. This makes up to a nice effect you can see in the video below:

```C++
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "../../include/tsprites.hpp"

int main(int argc, char **argv)
{
    char *spinner[] = { 
                        (char*) " - ▁   SSprite ", 
                        (char*) " \\ ▂ ░ SSprite ",
                        (char*) " | ▃ ▒ SSprite ",
                        (char*) " / ▄ ▓ SSprite ",
                        (char*) " - ▅ █ SSprite ",
                        (char*) " \\ ▆ ▓ SSprite ",
                        (char*) " | ▇ ▒ SSprite ",
                        (char*) " / █ ░ SSprite ",
    };

    rgb_color spinner_color = { 0x9C, 0x41, 0xdE };

    TSprite S;
    SSprite S2((char *)"_.:[M64]:._");
    SSprite S3(spinner, 8, spinner_color);

    unsigned int tick = 50;
    int char_tick = 0;
    unsigned int maxtick = 850;

    S.ImportFromFile((char*)"resources/demo7_188.unicode");

    board_init();

    while(tick < maxtick)
    {
        tick++;
        int x = 10 + 10*(sin( ((tick % 100)/100.0) * 6.28  ));
        cursor_home();
        cursor_right(x-1);
        S.Print();

        cursor_right(90);
        cursor_right((x-1)/4);
        S2.PrintUncolored();

        // spinner
        cursor_home();
        cursor_down(15);
        cursor_right(5);
        if(!(tick % 8)) S3.frame_idx = (++char_tick % 8);
        S3.Print();

        usleep(1000 * 10);
    }
    
    board_close();

    return 0;
}
```
https://github.com/M64GitHub/libtsprites/assets/84202356/90d4a9d3-815f-405c-beaa-802bda05cc45

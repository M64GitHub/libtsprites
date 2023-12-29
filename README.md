# libtsprites

... work in progress (see tsprites.h)

```C++
int main(int argc, char **argv)
{
    char *ifile_name = 0;

    // create a TSprite object S
    TSprite S;

    if (argc != 2) usage();       // assume a usage function, that exits

    ifile_name = argv[1];         // get filename from cmdline
    
    S.ImportFromFile(ifile_name); // import catimg redirected output file

    cursor_right(5);              // cursor movement functions are included
    cursor_up(S.h);               // move cursor up the height of the sprite

    S.Print(); // print the sprite!

    return 0;
}
```

![image](https://github.com/M64GitHub/libtsprites/assets/84202356/23279515-e2fc-46a8-a6d3-4a86daaf2b25)


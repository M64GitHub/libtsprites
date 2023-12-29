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

    printf("hello world!\n");
    
    S.ImportFromFile(ifile_name); // import catimg redirected output file

    cursor_right(5);              // cursor movement functions are included
    cursor_down(S.h);             // move cursor down to make space for the sprite
    cursor_up(S.h);               // and move cursor back up the height of the sprite

    S.Print(); // print the sprite!

    return 0;
}
```

![image](https://github.com/M64GitHub/libtsprites/assets/84202356/76831a6b-c8c9-4c75-ad3f-135b5c467697)


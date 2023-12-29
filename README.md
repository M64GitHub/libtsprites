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

    cursor_up(S.h);               // cursor movement functions are included

    S.Print(); // print the sprite!

    return 0;
}
```

![image](https://github.com/M64GitHub/libtsprites/assets/84202356/a9760ec4-bc91-4b44-95d2-0116805145b4)


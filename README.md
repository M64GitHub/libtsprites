# libtsprites

... work in progress (see tsprites.h)

```C++
int main(int argc, char **argv)
{
    char *ifile_name = 0;

    // TSprite object S
    TSprite S;

    printf("catimg to tsprites conversion/import test utility.\n");
    printf("M64, 2023.\n");

    if (argc != 2) usage(); // assume a usage function, that exits

    ifile_name = argv[1];

    // import catimg redirected output file
    S.ImportFromFile(ifile_name);

    printf ("\x1b[%dA", S.h);     // move cursor upwards height of TSprite

    S.Print(); // print it!

    return 0;
}
```


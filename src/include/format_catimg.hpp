// format_catimg.hpp - libtsprites, 2023-24 M64

#ifndef TSL_FORMAT_CATIMG_H
#define TSL_FORMAT_CATIMG_H

#define CATIMG_HDR 0x1b, 0x5b, 0x73, 0x1b, 0x5b, 0x3f, 0x32, 0x35, 0x6c
#define CATIMG_HDR_LEN 9

// "\x1b5b[m"  // reset all attributes
#define CATIMG_LINE_END 0x1b, 0x5b, 0x6d, 0x0a
#define CATIMG_LINE_END_LEN 4

// "\x1b[?25h" // make cursor visible
#define CATIMG_FILE_END 0x1b, 0x5b, 0x3f, 0x32, 0x35, 0x68
#define CATIMG_FILE_END_LEN 6

#define CATIMG_MAXSIZE (1024 * 256)

#endif

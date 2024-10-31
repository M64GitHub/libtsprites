#ifndef TS_TYPES_H
#define TS_TYPES_H

#include "tscolors.hpp"

typedef struct Point2D_s {
  int x;
  int y;
} Point2D_t;

typedef struct Point3D_s {
  int x;
  int y;
  int z;
} Point3D_t;

typedef struct Pixel2D_s {
  int x;
  int y;
  RGBColor_t c;
} Pixel2D_t;

typedef struct Pixel3D_s {
  int x;
  int y;
  int z;
  RGBColor_t c;
} Pixel3D_t;

typedef enum {
  TS_DIRECTION_UP,
  TS_DIRECTION_RIGHT,
  TS_DIRECTION_DOWN,
  TS_DIRECTION_LEFT
} TS_DIRECTION;

#endif

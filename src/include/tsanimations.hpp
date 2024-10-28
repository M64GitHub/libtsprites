// tsanimations.hpp - libtsprites, 2023-24 M64

#ifndef TSL_ANIMATIONS_H
#define TSL_ANIMATIONS_H

// structs and routines to handle animations and schedules

// animations available:


// Animation is sprite-type independent (stores only indices!)
typedef struct TSpriteAnimation_s {
  unsigned int tick = 0;
  unsigned int tick_divider = 1;

  int loop = 0;
  int loop_idx = 0;
  int loop_start = 0;
  int loop_end = 0;
} TSpriteAnimation_t;

#endif

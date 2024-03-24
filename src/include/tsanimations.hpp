// tsanimations.hpp - libtsprites, 2023-24 M64

#ifndef TSL_ANIMATIONS_H
#define TSL_ANIMATIONS_H

// Animation is sprite-type independent (stores only indices!)
typedef struct s_SpriteAnimation {
  char *name = 0;

  int *animation;  // array of frame indizes
  int *rel_x = 0;  // array relative x position to spr->x
  int *rel_y = 0;  // array relative y position to spr->y
  int ani_len = 0; // length of arrays

  int curren_idx = 0;

  int ctr1 = 0; // convenience counters and thresholds
  int ctr2 = 0;
  int ctr3 = 0;
  int thr1 = 0;
  int thr2 = 0;
  int thr3 = 0;

  int loop = 0;
  int loop_idx = 0;
  int loop_threshold = 0;
} SpriteAnimation;

#endif

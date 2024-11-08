// tsanimations.hpp - libtsprites, 2023-24 M64
// structs and routines to handle animations and schedules

#ifndef TSL_ANIMATIONS_H
#define TSL_ANIMATIONS_H

#include "tsrendersurface.hpp"

typedef enum AniStatus_e {
  TS_ANI_STATUS_STOPPED,
  TS_ANI_STATUS_PLAYING,
  TS_ANI_STATUS_PAUSED
} AniStatus_t;

// animations available:

// Animation is sprite-type independent (stores only indices!)
typedef struct TSpriteAnimation_s {
  unsigned int tick = 0;
  unsigned int tick_divider = 1;

  int loop = 0;
  int loop_start = 0;
  int loop_duration = 0;
  int loop_end = 0;
  int loop_idx_return = 0; // for looping back to idx > start
  int loop_idx_current = 0;
  RenderSurface_t *render_surface_out_bak =
      0; // for ani stop, restore original pointer to out_surface
  AniStatus_t status;
  int ani_idx = -1;
} TSpriteAnimation_t;

#endif

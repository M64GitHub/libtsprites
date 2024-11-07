// tsscener.hpp - libtsprites, 2023-24 M64

#ifndef TSL_SCENER_H
#define TSL_SCENER_H

#define TS_SCENES_MAX 128

typedef unsigned long tick_t;

typedef struct Scene_s {
  tick_t begin;
  tick_t duration;
  tick_t end;
  int id;
} Scene_t;

inline bool scene_check_in_scene(Scene_t *s, tick_t tick) {
  if ((tick > s->begin) && (tick < s->end))
    return true;

  return false;
}

inline bool scene_check_in_scene_full(Scene_t *s, tick_t tick) {
  if ((tick >= s->begin) && (tick <= s->end))
    return true;

  return false;
}

inline bool scene_check_at_scene_start(Scene_t *s, tick_t tick) {
  if (tick == s->begin)
    return true;

  return false;
}

inline bool scene_check_at_scene_end(Scene_t *s, tick_t tick) {
  if (tick == s->end)
    return true;

  return false;
}

inline tick_t scene_seconds_to_ticks(double seconds, double fps)
{
  double fps_divider = 1.0 / fps;
  return (tick_t)((double)(seconds / fps_divider) + 0.5);
}

void scene_init(Scene_t *s, tick_t begin, tick_t duration);
Scene_t scene_then(Scene_t *sc, tick_t duration);

#endif

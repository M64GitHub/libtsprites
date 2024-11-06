
// tsscener.cpp - libtsprites, 2023-24 M64

#include "include/tsscener.hpp"

void scene_init(Scene_t *s, tick_t begin, tick_t duration) {
  s->begin = begin;
  s->duration = duration;
  s->end = begin + duration;
}

Scene_t scene_then(Scene_t *s, tick_t duration) {
  Scene_t scene_return;
  scene_init(&scene_return, s->end + 1, duration);
  return scene_return;
}


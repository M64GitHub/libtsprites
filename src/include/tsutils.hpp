// tsutils.hpp - libtsprites, 2023-24 M64

#ifndef TSL_UTILS_H
#define TSL_UTILS_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define WINDOWS
#endif

#include "tscolors.hpp"

// -- time

extern int FPS; // TODO: handle this via stats struct and sstruct init function!
extern unsigned long fps_in_us;
unsigned long get_timestamp(struct timeval *tv);
unsigned long fps_to_us(int fps);
unsigned long us_to_fps(unsigned long us);
char *print_stats(unsigned long ts1, unsigned long ts2);

// -- cursor
void cursor_up(int n);
void cursor_down(int n);
void cursor_left(int n);
void cursor_right(int n);
void cursor_home();

void cursor_on();
void cursor_off();
void cursor_reset();

// -- color
void set_color(RGBColor_t c);
void set_bgcolor(RGBColor_t c);
void colorprintf(RGBColor_t c, const char *f, ...);

//-- term

enum {
  NB_ENABLE,
  NB_DISABLE
};

void term_init();
void term_close();
void term_clear();
int term_columns();
int term_rows();
void term_nonblock(int state);

// -- string
int strsize(char *s);
int mystrcpy(char *dest, char *src);
char *strdup(char *src);
void printhex(char *s);
void printhex(char *name, char *s);

// -- tools
void ruler(int n);
void idx_ruler(int n);

float min(float a, float b);
float max(float a, float b);

#endif

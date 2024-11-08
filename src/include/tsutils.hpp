// tsutils.hpp - libtsprites, 2023-24 M64

#ifndef TSL_UTILS_H
#define TSL_UTILS_H

#if defined(WIN32) || defined(_WIN32) ||                                       \
    defined(__WIN32) && !defined(__CYGWIN__)
#define WINDOWS
#endif

#include "tscolors.hpp"
#include <sys/time.h> // for gettimeofday
#include <termios.h>

#define KBD_KEY_UP 0x00415b1b
#define KBD_KEY_RIGHT 0x00435b1b
#define KBD_KEY_DOWN 0x00425b1b
#define KBD_KEY_LEFT 0x00445b1b

#define KBD_KEY_ESC 0x00001b
#define KBD_KEY_SPACE 0x000020

// -- time
typedef struct FPSCtx_s {
  int FPS;
  unsigned long FPS_us;
  unsigned long ts1;
  unsigned long ts2;
  struct timeval tv;
  unsigned long min;
  unsigned long max;
  int overruns;
} FPSCtx_t;

typedef struct KBCtx_s {
  struct timeval tv;
  fd_set fds;
  struct termios ttystate;
  int flags;
} KBCtx_t;

void fps_init(int FPS, FPSCtx_t *ctx);
void fps_begin_frame(FPSCtx_t *ctx);
void fps_end_frame(FPSCtx_t *ctx);

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

enum { NB_ENABLE, NB_DISABLE, E_ENABLE, E_DISABLE };

void term_init();
void term_close();
void term_clear();
int term_columns();
int term_rows();
void term_nonblock(int state, KBCtx_t *k);
void term_echo(int state);

void term_kbd_init(KBCtx_s *k);

unsigned int term_readkey();
int term_kbhit();

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

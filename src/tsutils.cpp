// tsutils.cpp - libtsprites, 2023-24 M64

#include "include/tsutils.hpp"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h> // for gettimeofday
#ifdef WINDOWS
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#define fileno _fileno
#define read _read
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

// -- time
// TODO: handle this via stats struct and sstruct init function!
int FPS = 0;
unsigned long fps_in_us = 0;

unsigned long get_timestamp(struct timeval *tv) {
  gettimeofday(tv, NULL);
  unsigned long r = 1000000 * tv->tv_sec + tv->tv_usec;
  return r;
}

unsigned long fps_to_us(int fps) {
  unsigned long r = 0.0f;
  unsigned long us1sec = 1 * 1000 * 1000;
  r = us1sec / fps;
  return r;
}

unsigned long us_to_fps(unsigned long us) {
  unsigned long r = 0;
  unsigned long us1sec = 1 * 1000 * 1000;
  r = us1sec / us;
  return r;
}

char *print_stats(unsigned long ts1, unsigned long ts2) {
  if (!FPS)
    return 0;
  // -- stats
  static unsigned long duration = 0;
  static unsigned long maxtime = 0;
  static unsigned long mintime = 10000000;
  static unsigned long maxfps = 0;
  static unsigned long minfps = 1000000;
  static char linestr[1024];
  static char outstr[8192];
  outstr[0] = 0x00;
  sprintf(linestr, "\x1b[0m");
  duration = (ts2 - ts1);
  if (duration > maxtime)
    maxtime = duration;
  if (duration < mintime)
    mintime = duration;
  if (us_to_fps(duration) > maxfps)
    maxfps = us_to_fps(duration);
  if (us_to_fps(duration) < minfps)
    minfps = us_to_fps(duration);
  sprintf(linestr, "FPS %d, ", FPS);
  strcat(outstr, linestr);
  // sprintf(linestr, "FPS duration in us: %lu, ", fps_in_us);
  // strcat(outstr, linestr);
  sprintf(linestr, "Cur duration in us: %lu, ", duration);
  strcat(outstr, linestr);
  sprintf(linestr, "Cur FPS: %lu, ", us_to_fps(duration));
  strcat(outstr, linestr);
  // sprintf(linestr, "Max duration in us: %lu ", maxtime);
  // strcat(outstr, linestr);
  // sprintf(linestr, "Min duration in us: %lu, ", mintime);
  // strcat(outstr, linestr);
  // sprintf(linestr, "Max FPS: %lu, ", maxfps);
  // strcat(outstr, linestr);
  // sprintf(linestr, "Min FPS: %lu, ", minfps);
  // strcat(outstr, linestr);
  sprintf(linestr, "framesleeping: %lu", fps_in_us - (ts2 - ts1));
  strcat(outstr, linestr);

  return outstr;
}

// -- cursor
void cursor_up(int n) { printf("\x1b[%dA", n); }

void cursor_down(int n) { printf("\x1b[%dB", n); }

void cursor_left(int n) { printf("\x1b[%dD", n); }

void cursor_right(int n) { printf("\x1b[%dC", n); }

void cursor_on() { printf("\x1b[?25h"); }

void cursor_off() { printf("\x1b[?25l"); }

void cursor_reset() {
  printf("\x1b[0m"); // reset all modes
}

void cursor_home() {
  printf("\x1b[H"); // home pos
}

// -- color

void set_color(RGBColor_t c) { printf("\x1b[38;2;%d;%d;%dm", c.r, c.g, c.b); }

void set_bgcolor(RGBColor_t c) { printf("\x1b[48;2;%d;%d;%dm", c.r, c.g, c.b); }

void colorprintf(RGBColor_t c, const char *f, ...) {
  printf("\x1b[38;2;%d;%d;%dm", c.r, c.g, c.b);
  va_list l;
  va_start(l, f);
  vprintf(f, l);
  va_end(l);
}

// -- term --

void term_init() {
  printf("\x1b[s");    // save cursor pos
  printf("\x1b[?47h"); // save screen
  printf("\x1b[2J");   // erase entire screen
  printf("\x1b[H");    // home pos
  cursor_off();
}

void term_clear() {
  printf("\x1b[2J"); // erase entire screen
  printf("\x1b[H");  // home pos
}

void term_close() {
  printf("\x1b[?47l"); // restore screen
  printf("\x1b[u");    // restore cursor pos
  cursor_on();
}

int term_columns() {
#ifdef WINDOWS
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  int ret;
  ret = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  return ret ? csbi.dwSize.X : 0;
#else
  struct winsize win;
  ioctl(1, TIOCGWINSZ, &win);
  return win.ws_col;
#endif
}

int term_rows() {
#ifdef WINDOWS
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  int ret;
  ret = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  return ret ? csbi.dwSize.Y : 0;
#else
  struct winsize win;
  ioctl(1, TIOCGWINSZ, &win);
  return win.ws_row;
#endif
}

// -- string

// returns size of string, includes 0x00 !
int strsize(char *str) {
  if (!str)
    return 0;

  int i = 0;
  while ((unsigned char)str[i++])
    ;

  return i;
}

int mystrcpy(char *dest, char *src) {
  if (!dest || !src)
    return 0;

  int i = 0;
  while ((unsigned char)src[i]) {
    dest[i] = src[i];
    i++;
  }
  dest[i] = 0x00;

  return i;
}

char *strdup(char *src) {
  if (!src)
    return 0;

  int l = strsize(src);
  char *tmpstr = (char *)calloc(l, 1);
  mystrcpy(tmpstr, src);

  return tmpstr;
}

// -- utils

void printhex(char *s) {
  int i = 0;
  RGBColor_t c = {0x80, 0x80, 0x80};
  RGBColor_t ce = {0xff, 0x80, 0x80};

  if (!s) {
    colorprintf(ce, "(null!)");
  }
  if (!s[0])
    colorprintf(ce, "(empty)");

  while ((unsigned char)s[i]) {
    colorprintf(c, "%02x ", (unsigned char)s[i++]);
  }
  colorprintf(ce, "00 ");
}

void printhex(char *name, char *s) {
  int i = 0;
  RGBColor_t c = {0x80, 0x80, 0x80};
  RGBColor_t ce = {0xff, 0x80, 0x80};
  RGBColor_t cn = {0xb0, 0xb0, 0xb0};

  colorprintf(cn, "%s ", name);

  if (!s) {
    colorprintf(ce, "(null!)");
  }
  if (!s[0])
    colorprintf(ce, "(empty)");

  while (s[i]) {
    colorprintf(c, "%02x ", (unsigned char)s[i++]);
  }
  colorprintf(ce, "00 ");
}

void ruler(int n) {
  char *line1 = (char *)"|   '    |    '    |    '    |    '    "
                        "|    '    |    '    |    '    |    '    "
                        "|    '    |    '    |    '    |    '    "
                        "|    '    |    '    |    '    |    '    "
                        "|    '    |    '    |    '    |    '    "
                        "|    '    |    '    |    '    |    '    "
                        "|    '    |    '    |    '    |    '    "
                        "|    '    |    '    |    '    |    '    "
                        "|    '    |    '    |    '    |    '    "
                        "|    '    |    '    |    '    |    '    "
                        "|    '   ";

  char *line2 = (char *)"1       10        20        30        4"
                        "0        50        60        70        8"
                        "0        90       100       110       12"
                        "0       130       140       150       16"
                        "0       170       180       190       17"
                        "0       210       220       230       24"
                        "0       250       260       270       28"
                        "0       290       300       310       32"
                        "0       330       340       350       "
                        "         ";
  int len = n;

  if (len > 359)
    len = 359;

  char *l1 = strdup(line1);
  char *l2 = strdup(line2);

  l1[len - 1] = '|';
  l1[len] = 0x00;
  char tmp[16];
  sprintf(tmp, "%d", len);
  sprintf(l2 + len - strsize(tmp) + 1, "%s", tmp);

  printf("%s", l1);
  cursor_left(strsize(l1) - 1);
  cursor_down(1);
  printf("%s\n", l2);

  free(l1);
  free(l2);
}

void idx_ruler(int n) {
  char *line1 =
      (char *)"*  '   '   '   |   '   '   '   |   '   '   '   |   '   '   '   "
              "|   '   '   '   |   '   '   '   |   '   '   '   |   '   '   '   "
              "|   '   '   '   |   '   '   '   |   '   '   '   |   '   '   '   "
              "|   '   '   '   |   '   '   '   |   '   '   '   |   '   '   '   "
              "|   '   '   '   ";

  char *line2 =
      (char *)"0             0f              1f              2f              3"
              "f              4f              5f              6f              7"
              "f              8f              9f              af              b"
              "f              cf              df              ef              f"
              "f               ";

  int len = n;

  if (len > 256)
    len = 256;

  char *l1 = strdup(line1);
  char *l2 = strdup(line2);

  l1[len - 1] = '|';
  l1[len] = 0x00;
  char tmp[16];
  sprintf(tmp, " %02x ", len - 1);
  sprintf(l2 + len - 3, "%s", tmp);

  printf("%s", l1);
  cursor_left(strsize(l1) - 1);
  cursor_down(1);
  printf("%s\n", l2);

  free(l1);
  free(l2);
}

// --

float min(float a, float b) { return a <= b ? a : b; }
float max(float a, float b) { return a >= b ? a : b; }

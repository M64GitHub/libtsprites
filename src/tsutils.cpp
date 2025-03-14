// tsutils.cpp - libtsprites, 2023-24 M64

#include "include/tsutils.hpp"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WINDOWS
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#define fileno _fileno
#define read _read
#else
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

// -- time
void fps_init(int FPS, FPSCtx_t *ctx) {
  ctx->FPS = FPS;
  ctx->FPS_us = fps_to_us(FPS);
  ctx->min = 9999999999;
  ctx->max = 0;
  ctx->overruns = 0;
  ctx->ts2 = get_timestamp(&ctx->tv);
}

void fps_begin_frame(FPSCtx_t *ctx) { ctx->ts1 = get_timestamp(&ctx->tv); }

void fps_end_frame(FPSCtx_t *ctx) {
  //  ctx->ts1 = ctx->ts2;
  ctx->ts2 = get_timestamp(&ctx->tv);
  unsigned long duration = ctx->ts2 - ctx->ts1;
  if (duration < ctx->min)
    ctx->min = duration;
  if (duration > ctx->max)
    ctx->max = duration;
  if (duration > ctx->FPS_us) {
    ctx->overruns++;
  }
  if ((ctx->FPS_us - duration) > 200)
    duration -= 200;
  usleep(ctx->FPS_us - (duration) < ctx->FPS_us ? ctx->FPS_us - (duration) : 1);
}

unsigned long fps_to_us(int fps) {
  unsigned long r = 0;
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

unsigned long get_timestamp(struct timeval *tv) {
  gettimeofday(tv, NULL);
  unsigned long r = 1000000 * tv->tv_sec + tv->tv_usec;
  return r;
}

char *print_stats(FPSCtx_t *ctx) {
  if (!ctx->FPS)
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
  duration = (ctx->ts2 - ctx->ts1);
  if (duration > maxtime)
    maxtime = duration;
  if (duration < mintime)
    mintime = duration;
  if (us_to_fps(duration) > maxfps)
    maxfps = us_to_fps(duration);
  if (us_to_fps(duration) < minfps)
    minfps = us_to_fps(duration);
  sprintf(linestr, "FPS %d, ", ctx->FPS);
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
  sprintf(linestr, "framesleeping: %lu", ctx->FPS_us - (ctx->ts2 - ctx->ts1));
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

int term_kbhit() {
  struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds); // STDIN_FILENO is 0
  select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &fds);
}

void term_kbd_init(KBCtx_s *k) {
  int nflags;

  // get the terminal state
  tcgetattr(STDIN_FILENO, &k->ttystate);

  k->ttystate.c_lflag &= ~ICANON;
  k->ttystate.c_cc[VMIN] = 1;
  tcsetattr(STDIN_FILENO, TCSANOW, &k->ttystate);

  k->flags = fcntl(STDIN_FILENO, F_GETFL);
  nflags = k->flags;
  nflags |= O_NONBLOCK;
  fcntl(STDIN_FILENO, F_SETFL, nflags);
};

void term_nonblock(int state, KBCtx_t *k) {
  int nflags;

  if (state == NB_ENABLE) {
    nflags = k->flags;
    nflags |= O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, nflags);
  } else if (state == NB_DISABLE) {
    nflags = k->flags;
    nflags &= ~O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, nflags);
  }
}

void term_echo(int state) {
  struct termios ttystate;

  tcgetattr(STDIN_FILENO, &ttystate);

  if (state == E_DISABLE) {
    ttystate.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

  } else if (state == E_ENABLE) {
    ttystate.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
  }
}

unsigned int term_readkey() {
  char KBUF[8];
  *(int *)KBUF = 0x00;
  fgets(KBUF, 4, stdin);

  return KBUF[0] + (KBUF[1] << 8) + (KBUF[2] << 16);
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

double ts_ease_in(int tick, int tick_maximum, double y_max) {
  double t = (double)tick / (double)tick_maximum;
  if (t > 1.0) {
    t = 1.0;
  }
  return t * t * y_max;
}

double ts_ease_in_out(int tick, int tick_maximum, double y_max) {
  double t = (double)tick / (double)tick_maximum;
  if (t > 1.0) {
    t = 1.0;
  }
  if (t < 0.5) {
    return 2 * t * t * y_max;
  } else {
    return (1 - 2 * (1 - t) * (1 - t)) * y_max;
  }
}


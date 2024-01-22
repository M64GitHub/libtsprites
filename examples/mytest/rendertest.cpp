// Moving spaceship example
// rendertest.cpp - libtsprites, 2023, M64

#include "../../include/tscolors.hpp"
#include "../../include/tscreen.hpp"
#include "../../include/tsprites.hpp"
#include "../../include/tsrender.hpp"
#include "../../include/tsutils.hpp"
#include <math.h>     // for sin()
#include <stdio.h>    // for printf
#include <sys/time.h> // for gettimeofday
#include <unistd.h>   // for usleep()

int FPS = 60;

// -- stats
unsigned long duration = 0;
unsigned long maxtime = 0;
unsigned long mintime = 1000000;
unsigned long maxfps = 0;
unsigned long minfps = 100000;
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

void print_stats(unsigned long ts1, unsigned long ts2) {
  printf("\x1b[0m");
  duration = (ts2 - ts1);
  if (duration > maxtime)
    maxtime = duration;
  if (duration < mintime)
    mintime = duration;
  if (us_to_fps(duration) > maxfps)
    maxfps = us_to_fps(duration);
  if (us_to_fps(duration) < minfps)
    minfps = us_to_fps(duration);
  cursor_home();
  cursor_down(46);
  printf("\nFPS %d    \n", FPS);
  printf("FPS duration in us: %lu    \n", fps_in_us);
  printf("Cur duration in us: %lu    \n", duration);
  printf("Cur FPS: %lu    \n", us_to_fps(duration));
  printf("Max duration in us: %lu    \n", maxtime);
  printf("Min duration in us: %lu    \n", mintime);
  printf("Max FPS: %lu    \n", maxfps);
  printf("Min FPS: %lu    \n", minfps);
  printf("framesleeping: %lu                   \n", fps_in_us - (ts2 - ts1));
}

int main(int argc, char **argv) {
  struct timeval tv;
  unsigned long ts1 = 0;
  unsigned long ts2 = 0;
  unsigned long ts3 = 0;
  fps_in_us = fps_to_us(FPS);

  char *spinners[] = {
      (char *)" - ▁   SSprite ", (char *)" \\ ▂ ░ SSprite ",
      (char *)" | ▃ ▒ SSprite ", (char *)" / ▄ ▓ SSprite ",
      (char *)" - ▅ █ SSprite ", (char *)" \\ ▆ ▓ SSprite ",
      (char *)" | ▇ ▒ SSprite ", (char *)" / █ ░ SSprite ",
  };
  rgb_color spinners_color = {0x9C, 0x41, 0xdE}; // R, G, B

  TSRenderEngineTopDown engine;

  TScreen Screen(120, 46);
  Screen.bg_color = {0x10, 0x10, 0x10};
  Screen.SetRenderEngine(&engine);
  Screen.SetScreenMode(SCREEN_BGCOLOR);

  TSprite SprDemo;
  TSprite SprTSprites;
  TSprite SpcShip1;
  TSprite SpcShip2;
  SprDemo.ImportFromFile((char *)"resources/demo7t_188.unicode");
  SprTSprites.ImportFromFile((char *)"resources/demo1.unicode");
  SpcShip1.ImportFromFile((char *)"resources/spc.unicode");
  SpcShip2.ImportFromFile((char *)"resources/spc.unicode");

  Screen.AddSprite(&SpcShip1);
  Screen.AddSprite(&SprDemo);
  Screen.AddSprite(&SpcShip2);
  Screen.AddSprite(&SprTSprites);

  SSprite Spinner(spinners, 8, spinners_color);
  Screen.AddSprite(&Spinner);
  Spinner.x = 1;
  Spinner.y = 44 * 2;

  // -- SubScreen
  TScreen SubScreen(20, 20); // height in lines
  SubScreen.is_subscreen = 1;
  SubScreen.SetScreenMode(SCREEN_BGCOLOR);
  SubScreen.bg_color = {0x20, 0x28, 0x30};
  SubScreen.SetXY(98, 46 * 2 - 2 - 40);

  TSprite SpcShip3;
  SpcShip3.ImportFromFile((char *)"resources/spc.unicode");
  SpcShip3.SetXY(3, 2);

  SSprite S1((char *)"SubScreen");
  rgb_color S1Color = {0xdf, 0xdf, 0xdf};
  S1.x = 11;
  S1.y = 19 * 2;
  S1.frames[0]->color = S1Color;
  S1.background = &SubScreen.bg_color; // textcolor changes with scrn color

  SubScreen.AddSprite(&S1);
  SubScreen.AddSprite(&SprDemo);

  Screen.AddSubScreen(&SubScreen);

  // -- main loop
  int x = 0;
  int y = 0;
  int x2 = 0;
  int y2 = 0;
  int x3 = 0;
  int y3 = 0;
  int x4 = 0;
  int y4 = 0;
  rgb_color s1bgcolor = {0x00, 0x00, 0x00};
  while (1) {
    ts1 = get_timestamp(&tv);
    SpcShip1.counter1++;

    x = 53 + 50 * (sin(((SpcShip1.counter1 % 100) / 100.0) * 6.28)) - 1;
    y = 30 + 10 * (cos(((SpcShip1.counter1 % 100) / 100.0 * 3) * 6.28));

    x3 = 46 + 40 * (sin((((SpcShip1.counter1 + 50) % 100) / 100.0) * 6.28)) - 1;
    y3 = 30 + 8 * (cos((((SpcShip1.counter1 + 50) % 100) / 100.0 * 3) * 6.28));

    x2 = 5 + 25 * (sin(((SpcShip1.counter1 % 100) / 100.0) * 6.28)) + 10;
    y2 = 20 + 10 * (cos(((SpcShip1.counter1 % 100) / 100.0) * 6.28));

    x4 = 12 + 14 * (sin((((SpcShip1.counter1) % 100) / 100.0) * 6.28)) - 2;
    y4 = y3 + 15;

    SpcShip1.SetXY(x, y);
    SpcShip2.SetXY(x3, y3);
    SprDemo.SetXY(x2, y2);
    SprTSprites.SetXY(x4, y4);

    SubScreen.Render();
    Screen.Render();

    if (!(Spinner.counter1++ % 4))
      Spinner.frame_idx = (Spinner.counter2++ % 8);

    ts2 = get_timestamp(&tv);
    print_stats(ts1, ts2);

    // -- wait until full frame time reached
    ts3 = get_timestamp(&tv);
    usleep(fps_in_us - (ts3 - ts1) < fps_in_us ? fps_in_us - (ts3 - ts1) : 1);
  }

  return 0;
}

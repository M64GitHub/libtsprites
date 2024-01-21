// Moving spaceship example
// rendertest.cpp - libtsprites, 2023, M64

#include "../../include/tscolors.hpp"
#include "../../include/tscreen.hpp"
#include "../../include/tsprites.hpp"
#include "../../include/tsrender.hpp"
#include "../../include/tsutils.hpp"
#include <math.h> // for sin()
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h> // for usleep()

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

// --

int main(int argc, char **argv) {
  struct timeval tv;
  unsigned long ts1 = 0;
  unsigned long ts2 = 0;
  unsigned long ts3 = 0;
  fps_in_us = fps_to_us(FPS);

  TSRenderEngineTopDown engine;

  rgb_color S1Color = {0xdf, 0xdf, 0xdf}; // R, G, B
  SSprite S1((char *)"SubScreen");
  S1.frames[0]->color = S1Color;

  char *spinners[] = {
      // array of strings for the spinner sprite below
      (char *)" - ▁   SSprite ", (char *)" \\ ▂ ░ SSprite ",
      (char *)" | ▃ ▒ SSprite ", (char *)" / ▄ ▓ SSprite ",
      (char *)" - ▅ █ SSprite ", (char *)" \\ ▆ ▓ SSprite ",
      (char *)" | ▇ ▒ SSprite ", (char *)" / █ ░ SSprite ",
  };

  rgb_color spinners_color = {0x9C, 0x41, 0xdE}; // R, G, B
  SSprite S2(spinners, 8, spinners_color);       // create a string sprite,

  TScreen Screen(120, 46);
  Screen.bg_color = {0x10, 0x10, 0x10};
  Screen.SetRenderEngine(&engine);
  Screen.SetScreenMode(SCREEN_BGCOLOR); // was SCREEN_TRANSPARENT

  TSprite SprDemo;
  TSprite SprDemo2;
  TSprite SpcShip;
  TSprite SpcShip2;
  SprDemo.ImportFromFile((char *)"resources/demo7t_188.unicode");
  SprDemo2.ImportFromFile((char *)"resources/demo1.unicode");
  SpcShip.ImportFromFile((char *)"resources/spc.unicode");
  SpcShip2.ImportFromFile((char *)"resources/spc.unicode");

  Screen.AddSprite(&SpcShip);
  Screen.AddSprite(&SprDemo);
  Screen.AddSprite(&SpcShip2);
  Screen.AddSprite(&SprDemo2);

  // -- SubScreen
  TSprite SpcShip3;
  TScreen SubScreen(20, 20);
  SpcShip3.ImportFromFile((char *)"resources/spc.unicode");
  SpcShip3.Render();
  SpcShip3.SetXY(3, 2);
  SubScreen.is_subscreen = 1;
  Screen.AddSubScreen(&SubScreen);
  SubScreen.SetScreenMode(SCREEN_BGCOLOR);
  SubScreen.bg_color = {0x20, 0x28, 0x30};
  SubScreen.SetXY(98, 46 * 2 - 2 - 40);
  SubScreen.AddSprite(&SpcShip3);
  SubScreen.AddSprite(&SprDemo);
  SubScreen.Render();

  // -- main loop

  int x = 0;
  int y = 0;
  int x2 = 0;
  int y2 = 0;
  int x3 = 0;
  int y3 = 0;
  int x4 = 0;
  int y4 = 0;

  while (1) {
    ts1 = get_timestamp(&tv);
    SpcShip.counter1++;

    x = 53 + 50 * (sin(((SpcShip.counter1 % 100) / 100.0) * 6.28)) - 1;
    y = 30 + 10 * (cos(((SpcShip.counter1 % 100) / 100.0 * 3) * 6.28));

    x3 = 46 + 40 * (sin((((SpcShip.counter1 + 50) % 100) / 100.0) * 6.28)) - 1;
    y3 = 30 + 8 * (cos((((SpcShip.counter1 + 50) % 100) / 100.0 * 3) * 6.28));

    x2 = 5 + 25 * (sin(((SpcShip.counter1 % 100) / 100.0) * 6.28)) + 10;
    y2 = 20 + 10 * (cos(((SpcShip.counter1 % 100) / 100.0) * 6.28));

    x4 = 12 + 14 * (sin((((SpcShip.counter1) % 100) / 100.0) * 6.28)) - 2;
    y4 = y3 + 15;

    SpcShip.SetXY(x, y);
    SpcShip2.SetXY(x3, y3);
    SprDemo.SetXY(x2, y2);
    SprDemo2.SetXY(x4, y4);

    SubScreen.Render();
    Screen.Render();

    cursor_home();
    cursor_down(44);
    cursor_right(4);
    if (!(S2.counter1++ % 4))
      S2.frame_idx = (S2.counter2++ % 8);
    S2.Print(); // print it
    cursor_right(90);
    S1.Print();
    fflush(stdout);

    ts2 = get_timestamp(&tv);

    // --
    // time for user stuff
    //
    // print_stats(ts1, ts2);
    // --

    //
    // -- wait until full frame time reached
    ts3 = get_timestamp(&tv);
    usleep(fps_in_us - (ts3 - ts1) < fps_in_us ? fps_in_us - (ts3 - ts1) : 1);
  }

  // --

  return 0;
}

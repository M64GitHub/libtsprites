// test.cpp - libtsprites, 2023, M64

#include "../../include/tscolors.hpp"
#include "../../include/tscreen.hpp"
#include "../../include/tsprites.hpp"
#include "../../include/tsrender.hpp"
#include "../../include/tsutils.hpp"
#include <math.h>     // for sin()
#include <sys/time.h> // for gettimeofday
#include <unistd.h>   // for usleep()

int FPS = 60;                // set stable FPS
unsigned long fps_in_us = 0; // will be calculated

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

int main(int argc, char **argv) {
  struct timeval tv;
  unsigned long ts1 = 0;
  unsigned long ts2 = 0;
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
  Screen.SetScreenMode(SCREEN_BGCOLOR); // default: SCREEN_TRANSPARENT

  TSprite SprDemo;
  TSprite SprTSprites;
  TSprite SpcShip1;
  TSprite SpcShip2;
  SprDemo.ImportFromFile((char *)"../resources/demo7t_188.unicode");
  SprTSprites.ImportFromFile((char *)"../resources/demo1.unicode");
  SpcShip1.ImportFromFile((char *)"../resources/spc.unicode");
  SpcShip2.ImportFromFile((char *)"../resources/spc.unicode");

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
  TSprite SpcShip3;
  SpcShip3.ImportFromFile((char *)"../resources/spc.unicode");
  SpcShip3.SetXY(3, 2);
  SubScreen.is_subscreen = 1;
  Screen.AddSubScreen(&SubScreen);
  SubScreen.SetScreenMode(SCREEN_BGCOLOR);
  SubScreen.bg_color = {0x20, 0x28, 0x30};
  SubScreen.SetXY(98, 46 * 2 - 2 - 40);
  SubScreen.AddSprite(&SpcShip3);
  SubScreen.AddSprite(&SprDemo);

  SSprite S1((char *)"SubScreen");
  SubScreen.AddSprite(&S1);
  rgb_color S1Color = {0xdf, 0xdf, 0xdf}; // R, G, B
  S1.x = 11;
  S1.y = 19 * 2;
  S1.frames[0]->color = S1Color;
  S1.background = &SubScreen.bg_color; // textcolor changes with scrn color

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
    ts1 = get_timestamp(&tv); // start measuring frame time
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

    // wait until full frame time reached
    ts2 = get_timestamp(&tv);
    usleep(fps_in_us - (ts2 - ts1) < fps_in_us ? fps_in_us - (ts2 - ts1) : 1);
  }

  return 0;
}

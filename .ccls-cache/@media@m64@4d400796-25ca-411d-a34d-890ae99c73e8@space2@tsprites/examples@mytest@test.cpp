// Moving spaceship example                                         2023, M64
#include "../../include/tscolors.hpp"
#include "../../include/tscreen.hpp"
#include "../../include/tsprites.hpp"
#include "../../include/tsrender.hpp"
#include "../../include/tsutils.hpp"
#include <math.h> // for sin()
#include <stdio.h>
#include <unistd.h> // for usleep()

#include <stdio.h>

int main(int argc, char **argv) {
  // rgb_color rgb = {82, 0, 87};
  // hsl_color hsl = {0, 0.0f, 0.0f};
  //
  // rgb2hsl(&rgb, &hsl);
  // printf("rgb2hsv: %d, %f, %f\n", hsl.h, hsl.s, hsl.l);
  //
  // rgb = {50, 50, 50};
  // hsl2rgb(&hsl, &rgb);
  // printf("hsl2rgb: %d, %d, %d\n", rgb.r, rgb.g, rgb.b);
  //
  // TSprite S;
  //
  // return 0;

  // --
  TScreen Screen(120, 45);

  TSprite S1;
  S1.ImportFromFile((char *)"resources/demo7t_188.unicode");

  SSprite SS1((char *)"lalala");

  TSprite SpcShip;
  SpcShip.ImportFromFile((char *)"resources/spaceship1_1.unicode");

  // SpcShip.PrintDebugMap(SpcShip.frames[0]);
  // return 0;

  ruler(120);

  // --

  int x = 0;
  int y = 0;
  int x2, y2;
  while (1) {
    Screen.CClear(' ');
    cursor_home();

    SS1.counter1++;
    SpcShip.Print();
    SpcShip.PrintDebugMap(SpcShip.fs.frames[0]);

    printf("x: %d, y: %d", x, y);
    fflush(stdout);

    x = 53 + 50 * (sin(((SS1.counter1 % 100) / 100.0) * 6.28)) - 1;
    y = 25 + 10 * (cos(((SS1.counter1 % 100) / 100.0 * 3) * 6.28)) - 1;

    x2 = 15 + 13 * (sin(((SS1.counter1 % 75) / 75.0) * 6.28)) - 1;
    y2 = 10 + 10 * (cos(((SS1.counter1 % 75) / 75.0) * 6.28)) - 1;
    S1.Print(x2, y2);
    fflush(stdout);

    SpcShip.Print(x, y + 40);
    fflush(stdout);

    // SS1.Print(x, y);

    usleep(1000 * 330);
  }

  // --

  // wait4keypress
  char buf16[16];
  char *tmpc;

  tmpc = fgets((char *)buf16, 1, stdin);

  return 0;
}

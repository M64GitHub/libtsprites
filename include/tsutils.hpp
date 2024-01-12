#ifndef TS_UTILS_H
#define TS_UTILS_H

#include "tscolors.hpp"

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
void set_color(rgb_color c);
void set_bgcolor(rgb_color c);
void colorprintf(rgb_color c, const char *f, ...);

//-- term
void term_init();
void term_close();
void term_clear();

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


#ifndef TS_UTILS_H
#define TS_UTILS_H

#include "tscolors.hpp"

void cursor_up(int n);
void cursor_down(int n);
void cursor_left(int n);
void cursor_right(int n);
void cursor_home();

void cursor_on();
void cursor_off();
void cursor_reset();

void set_color(rgb_color c);
void set_bgcolor(rgb_color c);
void colorprintf(rgb_color c, const char *f, ...);

void screen_init();
void screen_close();

int mystrlen(char *s);
int mystrcpy(char *dest, char *src);
char *strdup(char *src);

void ruler(int n);
void idx_ruler(int n);

#endif


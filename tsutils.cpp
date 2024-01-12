#include "include/tsutils.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// -- cursor

void cursor_up(int n) 
{
    printf ("\x1b[%dA", n);
}

void cursor_down(int n) 
{
    printf ("\x1b[%dB", n);
}

void cursor_left(int n) 
{
    printf ("\x1b[%dD", n);
}

void cursor_right(int n) 
{
    printf ("\x1b[%dC", n);
}

void cursor_on()
{
    printf("\x1b[?25h");
}

void cursor_off()
{
    printf("\x1b[?25l");
}

void cursor_reset()
{
    printf ("\x1b[0m"); // reset all modes
}

void cursor_home()
{
    printf("\x1b[H");   // home pos
}

// -- color

void set_color(rgb_color c)
{
    printf("\x1b[38;2;%d;%d;%dm", c.r, c.g, c.b);
}

void set_bgcolor(rgb_color c)
{
    printf("\x1b[48;2;%d;%d;%dm", c.r, c.g, c.b);
}

void colorprintf(rgb_color c, const char *f, ...)
{
    printf("\x1b[38;2;%d;%d;%dm", c.r, c.g, c.b);
    va_list l;
    va_start(l, f);
    vprintf(f, l);
    va_end(l);
}

// -- term -- 

void term_init()
{
    printf("\x1b[s");    // save cursor pos
    printf("\x1b[?47h"); // save screen
    printf("\x1b[2J");   // erase entire screen
    printf("\x1b[H");    // home pos
    cursor_off();
}

void term_clear()
{
    printf("\x1b[2J");   // erase entire screen
    printf("\x1b[H");    // home pos
}

void term_close()
{
    printf("\x1b[?47l"); // restore screen
    printf("\x1b[u");    // restore cursor pos
    cursor_on();
}

// -- string

// returns size of string, includes 0x00 !
int strsize(char *str)
{
    if(!str) return 0;

    int i=0;
    while((unsigned char)str[i++]);

    return i;
}

int mystrcpy(char *dest, char *src)
{
    if(!dest || !src) return 0;

    int i=0;
    while((unsigned char)src[i]) {
        dest[i]=src[i];
        i++;
    }
    dest[i] = 0x00;

    return i;
}

char *strdup(char *src)
{
    if(!src) return 0;

    int l = strsize(src);
    char *tmpstr = (char*) calloc(l, 1);
    mystrcpy(tmpstr, src);
    
    return tmpstr;
}

void printhex(char *s)
{
    int i=0;
    rgb_color c  { 0x80, 0x80, 0x80 };
    rgb_color ce { 0xff, 0x80, 0x80 };

    if(!s) {  colorprintf(ce, "(null!)"); }
    if(!s[0]) colorprintf(ce, "(empty)");

    while((unsigned char)s[i]) {
        colorprintf(c, "%02x ", (unsigned char)s[i++]);
    }
    colorprintf(ce, "00 ");
}

void printhex(char *name, char *s)
{
    int i=0;
    rgb_color c  { 0x80, 0x80, 0x80 };
    rgb_color ce { 0xff, 0x80, 0x80 };
    rgb_color cn { 0xb0, 0xb0, 0xb0 };

    colorprintf(cn, "%s ", name);

    if(!s) {  colorprintf(ce, "(null!)"); }
    if(!s[0]) colorprintf(ce, "(empty)");

    while(s[i]) {
        colorprintf(c, "%02x ", (unsigned char)s[i++]);
    }
    colorprintf(ce, "00 ");
}

// -- utils

void ruler(int n)
{
    char *line1 = (char *) 
         "|   '    |    '    |    '    |    '    "
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
        
    char *line2 = (char *) 
         "1       10        20        30        4"
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

    if (len > 359) len = 359;

    char *l1 = strdup(line1);
    char *l2 = strdup(line2);

    l1[len-1] = '|';
    l1[len] = 0x00;
    char tmp[16];
    sprintf(tmp, "%d", len);
    sprintf(l2+len-strsize(tmp)+1, "%s", tmp);
    
    printf("%s", l1); 
    cursor_left(strsize(l1)-1);
    cursor_down(1);
    printf("%s\n", l2); 

    free(l1);
    free(l2);
}


void idx_ruler(int n)
{
    char *line1 = (char *) 
         "*  '   '   '   |   '   '   '   |   '   '   '   |   '   '   '   "
        "|   '   '   '   |   '   '   '   |   '   '   '   |   '   '   '   "
        "|   '   '   '   |   '   '   '   |   '   '   '   |   '   '   '   "
        "|   '   '   '   |   '   '   '   |   '   '   '   |   '   '   '   "
        "|   '   '   '   ";
        
    char *line2 = (char *) 
         "0             0f              1f              2f              3"
        "f              4f              5f              6f              7"
        "f              8f              9f              af              b"
        "f              cf              df              ef              f"
        "f               ";

    int len = n;

    if (len > 256) len = 256;

    char *l1 = strdup(line1);
    char *l2 = strdup(line2);

    l1[len-1] = '|';
    l1[len] = 0x00;
    char tmp[16];
    sprintf(tmp, " %02x ", len-1);
    sprintf(l2+len-3, "%s", tmp);
    
    printf("%s", l1);
    cursor_left(strsize(l1)-1);
    cursor_down(1);
    printf("%s\n", l2);

    free(l1);
    free(l2);
}

// --

float min(float a, float b) {
	return a <= b ? a : b;
}

float max(float a, float b) {
	return a >= b ? a : b;
}


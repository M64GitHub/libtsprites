// tsprites.cpp - libtsprites, 2023-24 M64

#include "include/tsprites.hpp"
#include "include/format_catimg.hpp"
#include "include/tsutils.hpp"
#include <stdio.h>
#include <stdlib.h>

// -- TSprite -----------------------------------------------------------------
TSprite::TSprite() {}

// TSprite(char *imgstr)
// initialize from imgstr (catimg format), ie when included via 'xxd -i'
// make sure imgstr is 0x00 terminated!
TSprite::TSprite(char *imgstr) {
  if (ImportFromImgStr(imgstr))
    printf("[TS] ERROR: unable to convert imgstr.\n");
}

TSprite::~TSprite() { free_frames(); }

int TSprite::ImportFromImgStr(char *str) {
  if (!str) {
    printf("[TS][ImportFromImgStr] ERROR: input str null!\n");
    return 1;
  }

  char hdr[] = {CATIMG_HDR};
  int chk = 0;
  int width = 0;
  int height = 0;
  int l = 0;

  l = strsize(str);

  // -- check file "hdr": catimg esc seq 0x1b, 0x5b, 0x73 = "\x1b[s"
  if (l < CATIMG_HDR_LEN) {
    printf("[TS][ImportFromImgStr] ERROR: invalid file type!\n");
    return 1;
  }

  chk = 1;
  for (int i = 0; i < CATIMG_HDR_LEN; i++) {
    if (str[i] != hdr[i]) {
      chk = 0;
      break;
    }
  }
  if (!chk) {
    printf("[TS][ImportFromImgStr] ERROR: invalid file type!\n");
    return 1;
  }

  // -- start conversion
  int pos = CATIMG_HDR_LEN; // start of 1st line
  int lnr = 0;
  char *outstr = 0; // result, ready2print char * string, with relative
                    // line ends
  char buf8k[8192]; // 8k buf

  outstr = (char *)calloc(1, l + 4096); // surplus for new lineend encodings
                                        // 0x0a -> esc: go w left, 1 down
  if (!outstr) {
    printf("[TS][ImportFromImgStr] ERROR: unable to alloc tmp mem!\n");
    return 1;
  }

  // -- convert line by line
  int lpos = 0; // pos in line
  int pxcount = 0;
  unsigned int out_idx = 0;

  int i = 0;
  while (pos < (l - (CATIMG_LINE_END_LEN + CATIMG_FILE_END_LEN))) {
    // search end of line (0x0a)
    // s[pos] = start of line
    lpos = 0; // pos in line
    while (str[pos + lpos + CATIMG_LINE_END_LEN - 1] != 0x0a) {
      unsigned char c = str[pos + lpos];

      if (c == 0x96 || c == 0x20) {
        if (!lnr) {
          width++;
        } // count on 1st line only
        pxcount++;
      }
      lpos++;
    }

    // -- here we stand at line end, and have width:
    // -> add this line + go left + go down to result
    DBG("\n[TS][ImportFromImgStr] line nr #%d, pos: %d,"
        " llen: %d\n",
        lnr, pos, lpos);

    // copy line to result
    for (i = 0; i < lpos; i++)
      outstr[out_idx++] = str[pos + i];

    // create new, relative line end
    i = 0;
    sprintf(buf8k, "\x1b[0m"); // clear all modes
    while (buf8k[i]) {
      outstr[out_idx++] = buf8k[i++];
    }

    i = 0;
    sprintf(buf8k, "\x1b[%dD", width); // cursor go left(lpos)
    while (buf8k[i]) {
      outstr[out_idx++] = buf8k[i++];
    }

    i = 0;
    sprintf(buf8k, "\x1b[%dB", 1); // cursor go down(1)
    while (buf8k[i]) {
      outstr[out_idx++] = buf8k[i++];
    }

    lpos += CATIMG_LINE_END_LEN;

    lnr++;
    height += 2; // 1 char = 2 blocks high

    pos += lpos;
  }
  outstr[out_idx] = 0x00; // conversion done

  DBG("\nw[TS][ImportFromImgStr] x h = %d x %d = pxcount = %d,"
      " tt px-size of conversion: %d\n",
      width, height, pxcount, out_idx);

  // -- now we have w, h -> we know image size and can create and
  // fill maps / a new frame:

  TSPriteFrame *F = add_frames(1, width, height);
  F->s = strdup(outstr);

  // -- fill maps from input string
  imgstr_2maps(str, F);

  // -- create 1down representation, store in frame
  create_1down_str(F);

  // Skip, when called for further frames:
  // Sprite gets initialized only from 1st frame (frame 0)
  if (fs.frame_count == 1) {
    s = outstr;
    h = height;
    w = width;
    s_1down = strdup(F->s_1down);
  }

  return 0;
}

// I: filename
int TSprite::ImportFromFile(char *fn) {
  FILE *f;
  unsigned long f_size;
  char *f_contents = 0;

  if (!(f = fopen(fn, "rb"))) {
    printf("[TS][ImportFromFile] ERROR: can not open file.\n");
    return 1;
  }

  // -- get size
  fseek(f, 0, SEEK_END);
  f_size = ftell(f);
  if (f_size < CATIMG_HDR_LEN) {
    printf("[TS][ImportFromFile] ERROR: invalid file type! 1\n");
    fclose(f);
    return 1;
  }
  fseek(f, 0, SEEK_SET);
  DBG("[TS][ImportFromFile] file-size: %ld bytes\n", f_size);

  // -- max file size chk
  if (f_size > CATIMG_MAXSIZE) {
    printf("[TS][ImportFromFile] ERROR: invalid file! (too large)\n");
    fclose(f);
    return 1;
  }

  // -- read file
  f_contents = (char *)calloc(1, f_size + 1); // add 1 for 0x00 terminator
  if (f_contents == NULL) {
    printf("[TS][ImportFromFile] ERROR: unable to allocate memory"
           " for file.\n");
    fclose(f);
    return 1;
  }
  for (unsigned long i = 0; i < f_size; i++) {
    if (!fread(f_contents + i, 1, 1, f)) {
      printf("[TS][ImportFromFile] ERROR: unable to read file.\n");
      free(f_contents);
      fclose(f);
      return 1;
    }
  }
  f_contents[f_size] = 0x00; // terminate string properly
  DBG("[TS][ImportFromFile] successfully read file into buffer.\n");

  if (ImportFromImgStr(f_contents)) {
    printf("[TS][ImportFromFile] ERROR: unable to convert file.\n");
    free(f_contents);
    fclose(f);
    return 1;
  }

  free(f_contents);
  fclose(f);

  return 0;
}

TFrameset *TSprite::Slice(int swidth) { return 0; }

TFrameset *TSprite::Slice(int *swidths, int numslices) { return 0; }

void TSprite::Print() {
  if (!s)
    return;
  printf("%s", s);
  fflush(stdout);
}

void TSprite::Print(int X, int Y) {
  if (!s || !s_1down)
    return;
  cursor_home();
  if (Y > 1)
    cursor_down(Y / 2);
  if (X)
    cursor_right(X);

  if ((Y + 2) % 2)
    printf("%s", s_1down);
  else
    printf("%s", s);
  fflush(stdout);
}

void TSprite::PrintFrame(int n) {
  if (fs.frame_count < n)
    return;
  printf("%s", fs.frames[n]->s);
  fflush(stdout);
}

void TSprite::Prepare() {}

void TSprite::SetXY(int xx, int yy) {
  x = xx;
  y = yy;
  if (out_surface) {
    out_surface->x = xx;
    out_surface->y = yy;
  }
}

// renders current frame
render_surface *TSprite::Render() {
  if (!out_surface)
    return 0;

  TSPriteFrame *F = fs.frames[fs.frame_idx];

  for (int i = 0; i < (F->w * F->h); i++) {
    out_surface->colormap[i] = F->colormap[i];
    out_surface->shadowmap[i] = F->shadowmap[i];
  }

  out_surface->x = x;
  out_surface->y = y;

  return out_surface;
}

void TSprite::tick() {}

void TSprite::StartAnimation(int n, int loop) {}

void TSprite::PauseAnimation(int n) {}

void TSprite::StopAnimation(int n) {}

void TSprite::PrintDebugMap(TSPriteFrame *F) {
  rgb_color c = {0x80, 0x88, 0x88};

  colorprintf(c,
              "Frame #%d: COLOR:'o', TRANSPARENCY:'-' (with colors) "
              "w x h: %d x %d blocks\nblk ln map\n",
              F->nr, F->w, F->h);
  for (int Y = 0; Y < F->h; Y++) {
    colorprintf(c, "%02d, %02d ", Y, Y / 2);
    for (int X = 0; X < F->w; X++) {
      if (F->shadowmap[Y * F->w + X]) {
        printf("\x1b[0;38;2;%d;%d;%dm", F->colormap[Y * F->w + X].r,
               F->colormap[Y * F->w + X].g, F->colormap[Y * F->w + X].b);
        printf("o");
      } else {
        printf("\x1b[0;38;2;60;60;60m");
        printf("-");
      }
    }
    printf("\x1b[0m\n");
  }
  colorprintf(c, "       ");
  ruler(F->w);
  printf("\x1b[0m\n");
  printf("Frame #%d: COLOR: 'o', TRANSPARENCY: '-' (without colors) "
         "w x h: %d x %d blocks\nblk ln map\n",
         F->nr, F->w, F->h);
  for (int Y = 0; Y < F->h; Y++) {
    printf("%02d, %02d ", Y, Y / 2);
    for (int X = 0; X < F->w; X++) {
      if (F->shadowmap[Y * F->w + X]) {
        printf("o");
      } else {
        printf("-");
      }
    }
    printf("\n");
  }
  printf("\x1b[0m\n");
}

// --

// TSprite::imgstr_2maps
//
// helper for ImportFromImgStr: catimg line ends!
int TSprite::imgstr_2maps(char *str, TSPriteFrame *F) {
  unsigned int pos;
  int map_x, map_y;
  int res, r1, g1, b1, r2, g2, b2;
  r1 = 0;
  g1 = 0;
  b1 = 0;
  r2 = 0;
  g2 = 0;
  b2 = 0;
  int count = 0;
  rgb_color upper_color;
  rgb_color lower_color;

  pos = CATIMG_HDR_LEN;

  for (map_y = 0; map_y < (F->h / 2); map_y++) {
    for (map_x = 0; map_x < F->w; map_x++) {
      // -- case 1: doublepixel
      res = sscanf(str + pos, "\x1b[48;2;%d;%d;%dm\x1b[38;2;%d;%d;%dm\u2584",
                   &r1, &g1, &b1, &r2, &g2, &b2);
      if (res == 6) {
        count++;
        while ((unsigned char)str[pos] != 0x84)
          pos++;
        pos++;
        // write to maps
        upper_color.r = r1;
        upper_color.g = g1;
        upper_color.b = b1;
        lower_color.r = r2;
        lower_color.g = g2;
        lower_color.b = b2;
        F->colormap[(map_y * 2) * F->w + map_x] = upper_color;
        F->colormap[(map_y * 2 + 1) * F->w + map_x] = lower_color;
        F->shadowmap[(map_y * 2) * F->w + map_x] = 1;
        F->shadowmap[(map_y * 2 + 1) * F->w + map_x] = 1;
        continue;
      }

      // -- case 2 or 3:
      //    2: upper pixel, lower transparent: 0x80
      //    3: lower pixel, upper transparent: 0x84
      res = sscanf(str + pos, "\x1b[0;38;2;%d;%d;%dm", &r1, &g1, &b1);
      if (res == 3) {
        count++;
        int lookahead;
        int found = 0;
        for (lookahead = 14; lookahead < 25; lookahead++) {
          if ((unsigned char)str[pos + lookahead] == 0x84) {
            found = 0x84;
            pos += lookahead + 1;
            // write to maps
            lower_color.r = r1;
            lower_color.g = g1;
            lower_color.b = b1;
            upper_color.r = 0xF0; // transparent
            upper_color.g = 0x20; // transparent
            upper_color.b = 0x20; // transparent
            F->colormap[(map_y * 2) * F->w + map_x] = upper_color;
            F->colormap[(map_y * 2 + 1) * F->w + map_x] = lower_color;
            F->shadowmap[(map_y * 2) * F->w + map_x] = 0; // transparent
            F->shadowmap[(map_y * 2 + 1) * F->w + map_x] = 1;
            break;
          }
          if ((unsigned char)str[pos + lookahead] == 0x80) {
            found = 0x80;
            pos += lookahead + 1;
            // write to maps
            upper_color.r = r1;
            upper_color.g = g1;
            upper_color.b = b1;
            lower_color.r = 0xF0; // transparent
            lower_color.g = 0x20; // transparent
            lower_color.b = 0x20; // transparent
            F->colormap[(map_y * 2) * F->w + map_x] = upper_color;
            F->colormap[(map_y * 2 + 1) * F->w + map_x] = lower_color;
            F->shadowmap[(map_y * 2) * F->w + map_x] = 1;
            F->shadowmap[(map_y * 2 + 1) * F->w + map_x] = 0; // transparent
            break;
          }
        }
        if (!found) {
          printf("[TS] error creating maps at %d\n", pos);
          return 1;
        }
        continue;
      }

      // -- case 4: spc
      if ((unsigned char)str[pos + 3] == 0x20) {
        count++;
        pos += 4;
        // write to maps
        upper_color.r = 0xF0;
        upper_color.g = 0x20;
        upper_color.b = 0x20;
        lower_color.r = 0xF0;
        lower_color.g = 0x20;
        lower_color.b = 0x20;
        F->colormap[(map_y * 2) * F->w + map_x] = upper_color;
        F->colormap[(map_y * 2 + 1) * F->w + map_x] = lower_color;
        F->shadowmap[(map_y * 2) * F->w + map_x] = 0;
        F->shadowmap[(map_y * 2 + 1) * F->w + map_x] = 0;
        continue;
      }
      DBG("[TS] no case found at pos %d!\n", pos);
    }
    pos += 4;
  }

  // Initialize out_surface here, so it is not 0, and
  // Screen.AddSprite can be called
  if (!out_surface) {
    rgb_color c = {0x00, 0x00, 0x00};
    out_surface = new render_surface;
    init_surface(out_surface, F->w, F->h, c);
  }

  return 0;
}

// create_1down_str(TSPriteFrame *F)
// helper for ImportFromImgStr
// Uses the frame's color / shadow maps to create a position independent
// printable string, having the sprite moved down 1/2 a character.
char *TSprite::create_1down_str(TSPriteFrame *F) {
  char *tmpstr = (char *)calloc(1, strsize(F->s) * 2); // more than enough
  int tmpstr_idx = 0;
  char buf1k[4096];
  rgb_color upper;
  rgb_color lower;
  int i = 0;

  char *outstr = 0;

  // always print this "extra line"
  for (int X = 0; X < F->w; X++) {
    // uppest block row always transparent
    lower = F->colormap[X];
    // printf a top transparent color of map-row 0,
    // then line-end
    if (F->shadowmap[X]) // if there is a color
      sprintf(buf1k, "\x1b[0;38;2;%d;%d;%dm\u2584", lower.r, lower.g, lower.b);
    else
      sprintf(buf1k, "\e[m ");
    i = 0;
    while (buf1k[i])
      tmpstr[tmpstr_idx++] = buf1k[i++];
  }

  // relative line end
  i = 0;
  sprintf(buf1k, "\x1b[0m"); // clear all modes
  while (buf1k[i])
    tmpstr[tmpstr_idx++] = buf1k[i++];

  i = 0;
  sprintf(buf1k, "\x1b[%dD", F->w); // cursor go left(lpos)
  while (buf1k[i])
    tmpstr[tmpstr_idx++] = buf1k[i++];

  i = 0;
  sprintf(buf1k, "\x1b[%dB", 1); // cursor go down(1)
  while (buf1k[i])
    tmpstr[tmpstr_idx++] = buf1k[i++];
  tmpstr[tmpstr_idx] = 0x00; // terminator

  // h was initialized with +=2 -> always cleanly divisable
  for (int Y = 0; Y < ((F->h - 2) >> 1); Y++) {
    // -- all else lines
    for (int X = 0; X < F->w; X++) {
      lower = F->colormap[X + ((Y + 1) * 2) * F->w];
      upper = F->colormap[X + ((Y + 1) * 2 - 1) * F->w];

      if (!F->shadowmap[X + ((Y + 1) * 2 - 1) * F->w]) { // upper transp.
        if (!F->shadowmap[X + ((Y + 1) * 2) * F->w])     // and lower
          sprintf(buf1k, "\e[m ");
        else
          sprintf(buf1k, "\x1b[0;38;2;%d;%d;%dm\u2584", // only upper
                  lower.r, lower.g, lower.b);
      } else {
        if (!F->shadowmap[X + ((Y + 1) * 2) * F->w]) // lower transparent
          sprintf(buf1k, "\x1b[0;38;2;%d;%d;%dm\u2580", upper.r, upper.g,
                  upper.b);
        else
          sprintf(buf1k,
                  "\x1b[48;2;%d;%d;%dm\x1b[38;2;%d;%d;%dm\u2584", // none
                  upper.r, upper.g, upper.b, lower.r, lower.g, lower.b);
      }
      i = 0;
      while (buf1k[i])
        tmpstr[tmpstr_idx++] = buf1k[i++];
    }
    // relative line end
    i = 0;
    sprintf(buf1k, "\x1b[%dD", F->w); // cursor go left(lpos)
    while (buf1k[i])
      tmpstr[tmpstr_idx++] = buf1k[i++];

    i = 0;
    sprintf(buf1k, "\x1b[%dB", 1); // cursor go down(1)
    while (buf1k[i])
      tmpstr[tmpstr_idx++] = buf1k[i++];
    tmpstr[tmpstr_idx] = 0x00; // terminator
  }

  int Y = (F->h - 1);
  // do the line with n upper pixel, and last row of
  // colormap as upper color
  for (int X = 0; X < F->w; X++) {
    // lowest block row always transparent
    if (F->shadowmap[X + Y * F->w]) {
      upper = F->colormap[X + Y * F->w];
      // printf a top transparent color of map-row 0,
      // then line-end
      sprintf(buf1k, "\x1b[0;38;2;%d;%d;%dm\u2580", upper.r, upper.g, upper.b);
    } else {
      sprintf(buf1k, "\e[m ");
    }
    i = 0;
    while (buf1k[i])
      tmpstr[tmpstr_idx++] = buf1k[i++];
  }

  // relative line end
  i = 0;
  sprintf(buf1k, "\x1b[%dD", F->w); // cursor go left(lpos)
  while (buf1k[i])
    tmpstr[tmpstr_idx++] = buf1k[i++];

  i = 0;
  sprintf(buf1k, "\x1b[%dB", 1); // cursor go down(1)
  while (buf1k[i])
    tmpstr[tmpstr_idx++] = buf1k[i++];
  tmpstr[tmpstr_idx] = 0x00; // terminator

  outstr = strdup(tmpstr); // copies and truncates properly
  free(tmpstr);            // so we can free excess memory

  F->s_1down = outstr;

  return outstr;
}

// add_frames
// DOES allocate or initialize colormap / shadowmap!
// returns the 1st of appended / new frames
TSPriteFrame *TSprite::add_frames(int n, int width, int height) {
  if (n < 1)
    return 0;

  // build new frames
  TSPriteFrame **new_frames = new TSPriteFrame *[fs.frame_count + n];

  // copy old frames, if any
  for (int i = 0; i < fs.frame_count; i++)
    new_frames[i] = fs.frames[i];

  // append new frames to frameset
  for (int i = 0; i < n; i++) {
    TSPriteFrame *F = new TSPriteFrame;
    new_frames[fs.frame_count + i] = F;
    F->nr = fs.frame_count + i;
    F->colormap = new rgb_color[width * height];
    F->shadowmap = new unsigned char[width * height];
    F->w = width;
    F->h = height;
    F->s = 0;
    F->s_1down = 0;
  }
  // delete old array if one
  if (fs.frames)
    free(fs.frames);

  fs.frames = new_frames;
  fs.frame_count = fs.frame_count + n;

  return fs.frames[fs.frame_count - n];
}

void TSprite::free_frames() {}

// -- LSprite -----------------------------------------------------------------
// -- ASprite -----------------------------------------------------------------

// -- SSprite -----------------------------------------------------------------

SSprite::SSprite() {}

SSprite::SSprite(char *str) {
  SSPriteFrame *F = new SSPriteFrame;
  frames = new SSPriteFrame *[1];
  frames[0] = F;

  frames[0]->s = strdup(str);
  frames[0]->color.r = 255;
  frames[0]->color.g = 255;
  frames[0]->color.b = 255;

  s = strdup(frames[0]->s);

  frame_count = 1;
}

SSprite::SSprite(char **strs, int len) {
  frames = new SSPriteFrame *[len];
  int i = 0;

  for (int j = 0; j < len; j++) {
    int l = strsize(strs[j]);
    SSPriteFrame *F = new SSPriteFrame;
    frames[j] = F;

    char *tmpstr = (char *)calloc(l + 1, 1);
    DBG("[SS] strlen: %d\n", l);
    tmpstr[l] = 0x00; // extra safe
    for (i = 0; i < l; i++)
      tmpstr[i] = strs[j][i];

    frames[j]->s = tmpstr;
    frames[j]->color.r = 255;
    frames[j]->color.g = 255;
    frames[j]->color.b = 255;
  }

  frame_count = len;
}

SSprite::SSprite(char **strs, int len, rgb_color c) {
  frames = new SSPriteFrame *[len];
  int i = 0;

  for (int j = 0; j < len; j++) {
    int l = strsize(strs[j]);
    SSPriteFrame *F = new SSPriteFrame;
    frames[j] = F;

    char *tmpstr = (char *)calloc(l + 1, 1);
    DBG("[SS] strlen: %d\n", l);
    tmpstr[l] = 0x00; // extra safe
    for (i = 0; i < l; i++)
      tmpstr[i] = strs[j][i];

    frames[j]->s = tmpstr;
    frames[j]->color.r = c.r;
    frames[j]->color.g = c.g;
    frames[j]->color.b = c.b;
  }

  frame_count = len;
}

SSprite::SSprite(char **s, int len, rgb_palette p) {}

SSprite::~SSprite() {}

void SSprite::Print() {
  if (!frame_count)
    return;
  printf("\x1b[38;2;%d;%d;%dm", frames[frame_idx]->color.r,
         frames[frame_idx]->color.g, frames[frame_idx]->color.b);
  printf("%s", frames[frame_idx]->s);
}

void SSprite::Print(int X, int Y) {
  cursor_home();
  if (Y >= 2)
    cursor_down(Y >> 1);
  if (X)
    cursor_right(X);

  printf("%s", s);
  fflush(stdout);
}

void SSprite::PrintUncolored() {
  if (!frame_count)
    return;
  printf("%s", frames[frame_idx]->s);
}

void SSprite::PrintDimmed() {}

void SSprite::PrintFrame(int n) {}

// Renders current Frame + animations, etc to out_surface
void SSprite::free_frames() {}

void SSprite::Render() {}

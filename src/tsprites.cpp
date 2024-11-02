// tsprites.cpp - libtsprites, 2023-24 M64

#include "include/tsprites.hpp"
#include "include/format_catimg.hpp"
#include "include/lodepng.h"
#include "include/tscolors.hpp"
#include "include/tsrendersurface.hpp"
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

// -- Import Functions --------

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

  TSpriteFrame_t *F = add_frames(1, width, height);
  F->s = strdup(outstr);

  // -- fill maps from input string
  imgstr_2maps(str, F);
  // copy to frame output surface
  for (int i = 0; i < (F->w * F->h); i++) {
    F->out_surface->colormap[i] = F->colormap[i];
    F->out_surface->shadowmap[i] = F->shadowmap[i];
  }

  // -- create 1down representation, store in frame
  create_1down_str(F);

  // Skip, when called for further frames:
  // Sprite gets initialized only from 1st frame (frame 0)
  if (fs.frame_count == 1) {
    s = strdup(outstr);
    h = height;
    w = width;
    s_1down = strdup(F->s_1down);

    // Initialize out_surface here, so it is not 0, and
    // Screen.AddSprite can be called
    RGBColor_t c = {0x00, 0x00, 0x00};
    out_surface = new RenderSurface_t;
    restore_surface = new RenderSurface_t;

    render_surface_init(out_surface, w, h, c);
    render_surface_init(restore_surface, w, h, c);
    render_surface_copy(F->out_surface, out_surface);
    render_surface_copy(out_surface, restore_surface);
  }

  free(outstr); // was allocated with surplus, uncertainity of exact len

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

int TSprite::ImportFromPNGFile(char *fn) {
  unsigned int error;
  unsigned char *image = 0;
  unsigned png_width, png_height;

  printf("[TS][ImportFromPNGFile]\n");
  error = lodepng_decode32_file(&image, &png_width, &png_height, fn);

  if (error) {
    printf("[TS][ImportFromFile] ERROR: unable to convert from file: '%s'.\n",
           lodepng_error_text(error));
    return error;
  }

  TSpriteFrame_t *F = add_frames(1, png_width, png_height);
  for (int Y = 0; (unsigned)Y < png_height; Y++) {
    for (int X = 0; (unsigned)X < png_width; X++) {
      F->colormap[png_width * Y + X].r = image[(png_width * Y + X) * 4 + 0];
      F->colormap[png_width * Y + X].g = image[(png_width * Y + X) * 4 + 1];
      F->colormap[png_width * Y + X].b = image[(png_width * Y + X) * 4 + 2];

      F->shadowmap[png_width * Y + X] = image[(png_width * Y + X) * 4 + 3];
    }
  }

  Maps_2_UTF8(F);

  s = F->s;
  w = png_width;
  h = png_height;

  for (int i = 0; i < (F->w * F->h); i++) {
    F->out_surface->colormap[i] = F->colormap[i];
    F->out_surface->shadowmap[i] = F->shadowmap[i];
  }

  s_1down = strdup(F->s_1down);

  RGBColor_t c = {0x00, 0x00, 0x00};
  out_surface = new RenderSurface_t;
  restore_surface = new RenderSurface_t;

  render_surface_init(out_surface, w, h, c);
  render_surface_init(restore_surface, w, h, c);
  render_surface_copy(F->out_surface, out_surface);
  render_surface_copy(out_surface, restore_surface);

  free(image);

  return 0;
}

// -- Split Functions --------

// Split F, and append created frames to fs. Vertical cut line.
// Fixed width/height raster.
// Starts at x=0 / y=0.
// Returns index into fs of first new frame, or -1 on error.
// Use to cut spritesheet animations for example.
int TSprite::SplitFixedWH(TSpriteFrame_t *F, int swidth, int sheight) {
  if (!F)
    return 0;

  return 0;
}

// Split F, and append created frames to fs. Vertical cut line. Fixed width.
// Starts at x=0.
// Returns index into fs of first new frame, or -1 on error.
// fs.frame_count - returned index = number of added frames.
int TSprite::VSplitFixedW(TSpriteFrame_t *F, int swidth) {
  if (!F || (swidth < 1))
    return -1;

  int numslices = F->w / swidth + 1;

  int old_num_frames = fs.frame_count;

  TSpriteFrame_t *new_frame = 0;

  int xoffset = 0;
  for (int n = 0; n < numslices; n++) {
    int new_width = swidth;
    new_frame = add_frames(1, new_width, h);

    // fill colormap and shadow map
    for (int fy = 0; fy < F->h; fy++) {
      int new_x = 0;
      for (int fx = 0; fx < new_width; fx++) {
        if ((fx + xoffset) >= F->w)
          break; // skip if out of bounds

        new_frame->colormap[new_width * fy + new_x] =
            F->colormap[F->w * fy + fx + xoffset];
        new_frame->shadowmap[new_width * fy + new_x] =
            F->shadowmap[F->w * fy + fx + xoffset];
        new_x++;
      } // fx (width)
    } // fy (height)

    // copy to frame output surface
    for (int i = 0; i < (new_frame->w * new_frame->h); i++) {
      new_frame->out_surface->colormap[i] = new_frame->colormap[i];
      new_frame->out_surface->shadowmap[i] = new_frame->shadowmap[i];
    }
    xoffset += swidth;

    Maps_2_UTF8(F);
  } // n

  return old_num_frames;
}

// Split F, and append created frames to fs. Vertical cut line. Variable widths.
// Starts at x=0.
// Returns index into fs of first new frame, or -1 on error.
int TSprite::VSplit(TSpriteFrame_t *F, int *widths, int numslices) {
  if (!F || (numslices < 1) || !widths)
    return -1;

  int old_num_frames = fs.frame_count;

  TSpriteFrame_t *new_frame = 0;

  int xoffset = 0;
  for (int n = 0; n < numslices; n++) {
    int new_width = widths[n];
    new_frame = add_frames(1, new_width, h);

    // fill colormap and shadow map
    for (int fy = 0; fy < F->h; fy++) {
      int new_x = 0;
      for (int fx = 0; fx < new_width; fx++) {
        if ((fx + xoffset) >= F->w)
          break; // skip if out of bounds

        new_frame->colormap[new_width * fy + new_x] =
            F->colormap[F->w * fy + fx + xoffset];
        new_frame->shadowmap[new_width * fy + new_x] =
            F->shadowmap[F->w * fy + fx + xoffset];
        new_x++;
      } // fx (width)
    } // fy (height)

    // copy to frame output surface
    for (int i = 0; i < (new_frame->w * new_frame->h); i++) {
      new_frame->out_surface->colormap[i] = new_frame->colormap[i];
      new_frame->out_surface->shadowmap[i] = new_frame->shadowmap[i];
    }
    xoffset += widths[n];

    Maps_2_UTF8(F);
  } // n

  return old_num_frames;
}

// Split F and append created frames to fs. Vertical cut line. Variable widths.
// Starts at x=xoffsets[0]. Returns index into fs of first new frame, or -1 on
// Error. Use to split word-logo into single letters for example.
int TSprite::VSplit(TSpriteFrame_t *F, int *xoffsets, int *widths,
                    int numslices) {
  if (!F || (numslices < 1) || !xoffsets || !widths)
    return -1;

  int old_num_frames = fs.frame_count;

  TSpriteFrame_t *new_frame = 0;

  for (int n = 0; n < numslices; n++) {
    int new_width = widths[n];
    new_frame = add_frames(1, new_width, h);

    // fill colormap and shadow map
    for (int fy = 0; fy < F->h; fy++) {
      int new_x = 0;
      for (int fx = 0; fx < new_width; fx++) {
        if ((fx + xoffsets[n]) >= F->w)
          break; // skip if out of bounds

        new_frame->colormap[new_width * fy + new_x] =
            F->colormap[F->w * fy + fx + xoffsets[n]];
        new_frame->shadowmap[new_width * fy + new_x] =
            F->shadowmap[F->w * fy + fx + xoffsets[n]];
        new_x++;
      } // fx (width)
    } // fy (height)
    // copy to frame output surface
    for (int i = 0; i < (new_frame->w * new_frame->h); i++) {
      new_frame->out_surface->colormap[i] = new_frame->colormap[i];
      new_frame->out_surface->shadowmap[i] = new_frame->shadowmap[i];
    }

    Maps_2_UTF8(F);
  } // n

  return old_num_frames;
}

// Split F and append created frames to fs. Vertical cut line. Variable widths.
// Starts at x=xoffsets[0], yoffset. Returns index into fs of first new frame,
// or -1 on Error. Use to split word-logo into single letters for example.
int TSprite::VSplit(TSpriteFrame_t *F, int *xoffsets, int *widths, int yoffset,
                    int numslices) {
  if (!F || (numslices < 1) || !xoffsets || !widths)
    return -1;

  int old_num_frames = fs.frame_count;

  TSpriteFrame_t *new_frame = 0;

  for (int n = 0; n < numslices; n++) {
    int new_width = widths[n];
    new_frame = add_frames(1, new_width, h);

    // fill colormap and shadow map
    int new_y = 0;
    for (int fy = yoffset; fy < F->h; fy++) {
      int new_x = 0;
      for (int fx = 0; fx < new_width; fx++) {
        if ((fx + xoffsets[n]) >= F->w)
          break; // skip if out of bounds

        new_frame->colormap[new_width * new_y + new_x] =
            F->colormap[F->w * fy + fx + xoffsets[n]];
        new_frame->shadowmap[new_width * new_y + new_x] =
            F->shadowmap[F->w * fy + fx + xoffsets[n]];
        new_x++;
      } // fx (width)
      new_y++;
    } // fy (height)
    // copy to frame output surface
    for (int i = 0; i < (new_frame->w * new_frame->h); i++) {
      new_frame->out_surface->colormap[i] = new_frame->colormap[i];
      new_frame->out_surface->shadowmap[i] = new_frame->shadowmap[i];
    }

    Maps_2_UTF8(F);
  } // n

  return old_num_frames;
}

// Split and return array of newly created TSprite ptrs. Vertical cut line.
// Variable widths.
// Starts at x=xoffsets[0].
// Use to split word-logo into single letters for example.
TSprite **TSprite::VSplit2Sprites(TSpriteFrame_t *F, int *xoffsets, int *widths,
                                  int numslices) {
  // TODO: implement
  return 0;
}

// split and return as new animation
TSpriteAnimation_t TSprite::*Split2Ani(TSpriteFrame_t *F, int swidth,
                                       int sheight) {
  if (!F)
    return 0;
  // TODO: implement

  return 0;
}

TSpriteAnimation_t TSprite::*VSplit2Ani(TSpriteFrame_t *F, int swidth) {
  if (!F)
    return 0;
  // TODO: implement

  return 0;
}

TSpriteAnimation_t TSprite::*VSplit2Ani(TSpriteFrame_t *F, int *swidths,
                                        int numslices) {
  if (!F)
    return 0;
  // TODO: implement

  return 0;
}

void TSprite::Print() {
  if (!s) {
    return;
  }
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

// Render()
// Set current frame's out_surface as sprite's out_surface
// and sprite coordinates to the surface.
// (original spr out_surface in restore_surface)
RenderSurface_t *TSprite::Render() {
  if (!fs.frames)
    return 0;
  TSpriteFrame_t *F = fs.frames[fs.frame_idx];
  if (!F)
    return 0;
  if (!F->out_surface)
    return 0;

  out_surface = F->out_surface;
  out_surface->x = x;
  out_surface->y = y;

  return out_surface;
}

void TSprite::tick() {}

// control sprite internal animations
// TODO:
void AddAnimation(TSpriteAnimation_t *a) {};

// TODO:
void StartAnimation(int n, int loop) {};

// TODO:
void PauseAnimation(int n) {};

// TODO:
void StopAnimation(int n) {};

// TODO:
void AnimationTick(int n) {};

// control single frame animations
// TODO:
void AddFrameAnimation(TSpriteAnimation_t *a, TSpriteFrame_t *f) {};

// TODO:
void StartFrameAnimation(TSpriteFrame_t *f, int loop) {};

// TODO:
void PauseFrameAnimation(TSpriteFrame_t *f) {};

// TODO:
void StopFrameAnimation(int n) {};

// TODO:
void FrameAnimationTick(TSpriteFrame_t *f) {};

void TSprite::PrintDebugMap(TSpriteFrame_t *F) {
  RGBColor_t c = {0x80, 0x88, 0x88};

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
int TSprite::imgstr_2maps(char *str, TSpriteFrame_t *F) {
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
  RGBColor_t upper_color;
  RGBColor_t lower_color;

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
    pos += CATIMG_LINE_END_LEN;
  }

  return 0;
}

// create_1down_str(TSpriteFrame_t *F)
// helper for ImportFromImgStr
// Uses the frame's color / shadow maps to create a position independent
// printable string, having the sprite moved down 1/2 a character.
char *TSprite::create_1down_str(TSpriteFrame_t *F) {
  char *tmpstr = (char *)calloc(1, strsize(F->s) * 2); // more than enough
  int tmpstr_idx = 0;
  char buf1k[4096];
  RGBColor_t upper;
  RGBColor_t lower;
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
TSpriteFrame_t *TSprite::add_frames(int n, int width, int height) {
  if (n < 1)
    return 0;

  // build new frames
  TSpriteFrame_t **new_frames = new TSpriteFrame_t *[fs.frame_count + n];

  // copy old frames, if any
  for (int i = 0; i < fs.frame_count; i++) {
    new_frames[i] = fs.frames[i];
  }

  // append new frames to frameset
  for (int i = 0; i < n; i++) {
    TSpriteFrame_t *F = new TSpriteFrame_t;
    new_frames[fs.frame_count + i] = F;
    F->nr = fs.frame_count + i;
    F->colormap = new RGBColor_t[width * height];
    F->shadowmap = new unsigned char[width * height];
    F->w = width;
    F->h = height;
    F->s = 0;
    F->s_1down = 0;

    F->out_surface = new RenderSurface_t;
    render_surface_init(F->out_surface, F->w, F->h, {0, 0, 0});

    F->out_surface->x = x;
    F->out_surface->y = y;
  }
  // delete old array if one
  if (fs.frames)
    free(fs.frames);

  fs.frames = new_frames;
  fs.frame_count = fs.frame_count + n;

  return fs.frames[fs.frame_count - n];
}

int TSprite::UTF8_2_maps(char *str, TSpriteFrame_t *F) {
  // quick adaption from imgstr_2maps, first error checking OK
  // anyways, the function should never be needed.
  if (!str || !F)
    return 1;
  if (!F->colormap || !F->shadowmap)
    return 1;
  if (!F->h || !F->w)
    return 1;

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
  RGBColor_t upper_color;
  RGBColor_t lower_color;

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
    // pos += 4;
    while (((unsigned char)str[pos] != 'B') && str[pos])
      pos++;
    if (str[pos])
      pos++; // for case 'B'
  } // X

  return 0;
}

unsigned char *TSprite::Maps_2_UTF8(TSpriteFrame_t *F) {
  char buf1k[1024];
  int tmpstr_idx = 0;
  int i = 0;

  RGBColor_t upper;
  RGBColor_t lower;

  unsigned char *out_s;

  if (!F)
    return 0;
  if (!F->colormap)
    return 0;

  out_s = new unsigned char[F->w * F->h * 50];

  out_s[0] = 0x00; // terminator

  for (int Y = 0; Y < F->h; Y += 2) {
    // -- all else lines
    for (int X = 0; X < F->w; X++) {
      upper = F->colormap[X + (Y + 0) * F->w];
      lower = F->colormap[X + (Y + 1) * F->w];

      // upper transp.
      if (!F->shadowmap[X + (Y + 0) * F->w]) {
        if (!F->shadowmap[X + (Y + 1) * F->w]) // and lower
          sprintf(buf1k, "\e[m ");
        else
          sprintf(buf1k, "\x1b[0;38;2;%d;%d;%dm\u2584", // only upper
                  lower.r, lower.g, lower.b);
      } else {
        if (!F->shadowmap[X + (Y + 1) * F->w]) // lower transparent
          sprintf(buf1k, "\x1b[0;38;2;%d;%d;%dm\u2580", upper.r, upper.g,
                  upper.b);
        else
          sprintf(buf1k,
                  "\x1b[48;2;%d;%d;%dm\x1b[38;2;%d;%d;%dm\u2584", // none
                  upper.r, upper.g, upper.b, lower.r, lower.g, lower.b);
      }
      i = 0;
      while (buf1k[i])
        out_s[tmpstr_idx++] = buf1k[i++];
    }
    // relative line end
    i = 0;
    sprintf(buf1k, "\x1b[%dD", F->w); // cursor go left(lpos)
    while (buf1k[i])
      out_s[tmpstr_idx++] = buf1k[i++];

    i = 0;
    sprintf(buf1k, "\x1b[%dB", 1); // cursor go down(1)
    while (buf1k[i])
      out_s[tmpstr_idx++] = buf1k[i++];
    out_s[tmpstr_idx] = 0x00; // terminator
  }
  out_s[tmpstr_idx] = 0x00; // terminator

  unsigned char *ret_str;
  ret_str = (unsigned char *)strdup((char *)out_s);
  delete[] out_s;

  F->s = (char *)ret_str;
  create_1down_str(F);

  return ret_str;
}

void TSprite::free_frames() {}

// -- LSprite -----------------------------------------------------------------
// -- ASprite -----------------------------------------------------------------

// -- SSprite -----------------------------------------------------------------

SSprite::SSprite() {}

SSprite::SSprite(char *str) {
  SSpriteFrame_t *F = new SSpriteFrame_t;
  frames = new SSpriteFrame_t *[1];
  frames[0] = F;

  frames[0]->s = strdup(str);
  frames[0]->color.r = 255;
  frames[0]->color.g = 255;
  frames[0]->color.b = 255;

  s = strdup(frames[0]->s);

  frame_count = 1;
}

SSprite::SSprite(char **strs, int len) {
  frames = new SSpriteFrame_t *[len];
  int i = 0;

  for (int j = 0; j < len; j++) {
    int l = strsize(strs[j]);
    SSpriteFrame_t *F = new SSpriteFrame_t;
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

SSprite::SSprite(char **strs, int len, RGBColor_t c) {
  frames = new SSpriteFrame_t *[len];
  int i = 0;

  for (int j = 0; j < len; j++) {
    int l = strsize(strs[j]);
    SSpriteFrame_t *F = new SSpriteFrame_t;
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

SSprite::SSprite(char **s, int len, RGBPalette_t p) {}

SSprite::~SSprite() {}

void SSprite::SetText(char *t) { frames[frame_idx]->s = t; }

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

  if (background)
    printf("\x1b[48;2;%d;%d;%dm\x1b[38;2;%d;%d;%dm", background->r,
           background->g, background->b, frames[frame_idx]->color.r,
           frames[frame_idx]->color.g, frames[frame_idx]->color.b);
  else
    printf("\x1b[38;2;%d;%d;%dm", frames[frame_idx]->color.r,
           frames[frame_idx]->color.g, frames[frame_idx]->color.b);
  printf("%s", frames[frame_idx]->s);
  // printf("%s", s);
  fflush(stdout);
}

void SSprite::PrintUncolored() {
  if (!frame_count)
    return;
  printf("%s", frames[frame_idx]->s);
}

void SSprite::SetColor(RGBColor_t c) {
  frames[frame_idx]->color.r = c.r;
  frames[frame_idx]->color.g = c.g;
  frames[frame_idx]->color.b = c.b;
}

void SSprite::PrintDimmed() {}
void SSprite::free_frames() {}

void SSprite::Render() {}

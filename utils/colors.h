#ifndef COLOR_T
#define COLOR_T

typedef struct color
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
} color_t;

static color_t colors_red = {.r = 255, .g = 0, .b = 0};
static color_t colors_green = {.r = 0, .g = 255, .b = 0};
static color_t colors_blue = {.r = 0, .g = 0, .b = 255};
static color_t colors_black = {.r = 0, .g = 0, .b = 0};
static color_t colors_white = {.r = 255, .g = 255, .b = 255};

#else

extern color_t colors_red;
extern color_t colors_green;
extern color_t colors_blue;

#endif

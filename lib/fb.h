#include "../utils/colors.h"

void fb_init();
void fb_text(int x, int y, char *s, color_t col);
void fb_rect(int x, int y, unsigned int w, unsigned int h, color_t col);

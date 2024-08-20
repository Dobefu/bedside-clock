#include "../utils/colors.h"

void fb_init();
void fb_flip();
void fb_text(int x, int y, char *s, color_t col, unsigned short fs);
void fb_rect(int x, int y, unsigned w, unsigned h, color_t col);

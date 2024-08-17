#include "lib/fb.h"
#include "lib/uart.h"
#include "utils/colors.h"
#include "utils/delays.h"

int main()
{
  char c;
  int x = -1600;

  uart_init();
  fb_init();

  fb_rect(40, 45, 100, 100, colors_red);
  fb_rect(40, 190, 100, 100, colors_green);
  fb_rect(40, 335, 100, 100, colors_blue);

  fb_rect(600, 0, 200, 480, colors_white);

  while (1)
  {
    fb_text(x % (800 * 480), 0, "(<", colors_black);
    x++;
    fb_text(x % (800 * 480), 0, "(<", colors_white);
    wait_msec(1000);
  }
}

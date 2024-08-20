#include "lib/fb.h"
#include "lib/mbox.h"
#include "lib/uart.h"
#include "utils/colors.h"
#include "utils/delays.h"
#include "utils/rand.h"

int main()
{
  rand_init();
  uart_init();
  fb_init();

  fb_text(0, 0, "Test", colors_white, 1);
  fb_text(0, 6, "Test", colors_white, 2);
  fb_text(0, 8, "Test", colors_white, 4);
  fb_text(0, 9, "Test", colors_white, 8);
  fb_text(0, 9, "Test", colors_white, 16);
  wait_msec(1000000);

  while (1)
  {
    fb_rect(0, 480, 800, 480, colors_black);
    fb_text(0, 480, "Test", colors_white, 1);
    fb_flip();
    wait_msec(1000000);
    fb_rect(0, 0, 800, 480, colors_red);
    fb_text(0, 0, "Test", colors_white, 1);
    fb_flip();
    wait_msec(1000000);
    fb_rect(0, 480, 800, 480, colors_green);
    fb_text(0, 480, "Test", colors_black, 1);
    fb_flip();
    wait_msec(1000000);
    fb_rect(0, 0, 800, 480, colors_blue);
    fb_text(0, 0, "Test", colors_white, 1);
    fb_flip();
    wait_msec(1000000);
  }
}

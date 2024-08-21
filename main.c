#include "lib/fb.h"
#include "lib/mbox.h"
#include "lib/uart.h"
#include "utils/colors.h"
#include "utils/delays.h"
#include "utils/rand.h"

int x = 0;

int main()
{
  rand_init();
  uart_init();
  fb_init();

  while (1)
  {
    fb_rect(0, 0, 800, 480, colors_black);
    fb_text(x, 0, "Test", colors_white, 1);
    x = (x + 1) % 800;

    fb_flip();
    wait_msec(1000);
  }
}

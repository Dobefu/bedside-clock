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

  while (1)
  {
  }
}

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

  register unsigned long time_msec;
  unsigned long time_msec_prev = 0;
  unsigned long delta_msec = 0;

  while (1)
  {
    asm volatile("mrs %0, cntpct_el0" : "=r"(time_msec));

    fb_rect(0, 0, 800, 480, colors_black);

    for (int i = 0; i <= 16; i += 2)
    {
      fb_text(383, 228, "Test", colors_white, i);
    }

    fb_flip();
    delta_msec = time_msec - time_msec_prev;
    time_msec_prev = time_msec;

    printf("fps:       %d\n", 1000000 / (delta_msec / 60));
    wait_msec(16666 - (delta_msec / 1000));
  }
}

#include "lfb.h"
#include "power.h"
#include "uart.h"

int main()
{
  char c;

  uart_init();
  lfb_init();

  lfb_print(400, 240, "Test");

  while (1)
  {
    uart_puts(" 1 - power off\n");
    uart_puts(" 2 - reset\n");
    uart_puts(" Choose one: ");

    c = uart_getc();
    uart_send(c);
    uart_puts("\n\n");
    if (c == '1')
      power_off();
    if (c == '2')
      reset();
  }
}

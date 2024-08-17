#include "lib/fb.h"

int main()
{
  char c;
  int x;

  fb_init();

  while (1)
  {
    fb_print(x, 228, "Test");
  }
}

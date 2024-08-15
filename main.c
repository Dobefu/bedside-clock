#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "window.c"

struct WindowProps wp;

Display *d;
Window w;
GC gc;
int s;

int main(void)
{
  wp.height = 480;
  wp.width = 800;

  XEvent e;

  d = XOpenDisplay(NULL);

  if (d == NULL)
  {
    fprintf(stderr, "Cannot open display\n");
    return 1;
  }

  create_window(d, &w, s, wp);

  // Create a graphics context.
  gc = XCreateGC(d, w, 0, 0);

  // Set up input.
  XSelectInput(d, w, ExposureMask | KeyPressMask);
  XMapWindow(d, w);

  while (1)
  {
    XNextEvent(d, &e);

    if (e.type == Expose)
    {
      draw(d, w, s, wp, &gc);
    }

    // Close the window when escape is pressed.
    if (e.type == KeyPress && e.xkey.keycode == 61)
    {
      break;
    }
  }

  XCloseDisplay(d);
  return 0;
}

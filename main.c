#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

struct WindowProps wp;

int main(void)
{
  wp.height = 480;
  wp.width = 800;

  Display *d;
  Window w;
  XEvent e;

  int s;

  d = XOpenDisplay(NULL);

  if (d == NULL)
  {
    fprintf(stderr, "Cannot open display\n");
    return 1;
  }

  // Create the window.
  s = DefaultScreen(d);
  w = XCreateSimpleWindow(d, RootWindow(d, s), 0, 0, wp.width, wp.height, 0, BlackPixel(d, s), BlackPixel(d, s));

  // Disable resizing.
  XSizeHints *sh = XAllocSizeHints();
  sh->flags = PMinSize | PMaxSize;
  sh->min_height = sh->max_height = wp.height;
  sh->min_width = sh->max_width = wp.width;
  XSetWMSizeHints(d, w, sh, XA_WM_NORMAL_HINTS);
  XFree(sh);

  // Create a graphics context.
  GC gc = XCreateGC(d, w, 0, 0);

  // Set up input.
  XSelectInput(d, w, ExposureMask | KeyPressMask);
  XMapWindow(d, w);

  while (1)
  {
    XNextEvent(d, &e);

    if (e.type == Expose)
    {
      XSetForeground(d, gc, WhitePixel(d, s));
      XFillRectangle(d, w, gc, wp.width / 2 - 5, wp.height / 2 - 5, 10, 10);
      XFillRectangle(d, w, gc, 0, 0, 10, 10);
      XFillRectangle(d, w, gc, wp.width - 10, 0, 10, 10);
      XFillRectangle(d, w, gc, 0, wp.height - 10, 10, 10);
      XFillRectangle(d, w, gc, wp.width - 10, wp.height - 10, 10, 10);
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

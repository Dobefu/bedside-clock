#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include "main.h"

void create_window(Display *d, Window *w, int s, struct WindowProps wp)
{
  // Create the window.
  s = DefaultScreen(d);
  *w = XCreateSimpleWindow(d, RootWindow(d, s), 0, 0, wp.width, wp.height, 0, BlackPixel(d, s), BlackPixel(d, s));

  // Disable resizing.
  XSizeHints *sh = XAllocSizeHints();
  sh->flags = PMinSize | PMaxSize;
  sh->min_height = sh->max_height = wp.height;
  sh->min_width = sh->max_width = wp.width;
  XSetWMSizeHints(d, *w, sh, XA_WM_NORMAL_HINTS);
  XFree(sh);
}

void draw(Display *d, Window w, int s, struct WindowProps wp, GC *gc)
{

  XSetForeground(d, *gc, WhitePixel(d, s));
  XFillRectangle(d, w, *gc, wp.width / 2 - 5, wp.height / 2 - 5, 10, 10);
  XDrawString(d, w, *gc, wp.width / 2 - 5, wp.height / 2 - 5, "test", 4);
}

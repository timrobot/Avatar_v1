#include "AvieVideo.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>

struct MwmHints {
  unsigned long flags;
  unsigned long functions;
  unsigned long decorations;
  long input_mode;
  unsigned long status;
};

enum {
  MWM_HINTS_FUNCTIONS = (1L << 0),
  MWM_HINTS_DECORATIONS =  (1L << 1),

  MWM_FUNC_ALL = (1L << 0),
  MWM_FUNC_RESIZE = (1L << 1),
  MWM_FUNC_MOVE = (1L << 2),
  MWM_FUNC_MINIMIZE = (1L << 3),
  MWM_FUNC_MAXIMIZE = (1L << 4),
  MWM_FUNC_CLOSE = (1L << 5)
};

void *update_window(void *av_asvoid);

AvieVideo::AvieVideo(int width, int height, bool border) {
  w = width;
  h = height;

  /* open display */
  display = XOpenDisplay(NULL);

  /* try to match the visual info */
  XVisualInfo vinfo;
  XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);

  /* attributes of the window */
  XSetWindowAttributes attr;
  attr.colormap = XCreateColormap(display, DefaultRootWindow(display), vinfo.visual, AllocNone);
  attr.border_pixel = 0;
  attr.background_pixel = 0;

  /* create window with attributes */
  win = XCreateWindow(display, DefaultRootWindow(display), x, y, width, height, 0, vinfo.depth, InputOutput, vinfo.visual, CWColormap | CWBorderPixel | CWBackPixel, &attr);
  XSelectInput(display, win, StructureNotifyMask);

  /* create framebuffer */
  framebuffer = (uint32_t **)malloc(sizeof(uint32_t *) * width);
  for (int i = 0; i < width; i++) {
    framebuffer[i] = (uint32_t *)malloc(sizeof(uint32_t) * height);
    memset(framebuffer[i], 0, sizeof(uint32_t) * height);
  }

  /* graphics context */
  gc = XCreateGC(display, win, 0, 0);

  /* delete border */
  if (!(border_en = border)) {
    Atom mwmHintsProperty = XInternAtom(display, "_MOTIF_WM_HINTS", 0);
    struct MwmHints hints;
    hints.flags = MWM_HINTS_DECORATIONS;
    hints.decorations = 0;
    XChangeProperty(display, win, mwmHintsProperty, mwmHintsProperty, 32,
        PropModeReplace, (unsigned char *)&hints, 5);
  }

  /* close window atom */
  Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
  XSetWMProtocols(display, win, &wm_delete_window, 1);

  /* map window */
  XMapWindow(display, win);
  display_en = true;
}

AvieVideo::~AvieVideo() {
  if (display_en) {
    display_en = false;
    XDestroyWindow(display, win);
    XCloseDisplay(display);
  }
}

bool AvieVideo::flush() {
  /* if (game.exit_signal) quit() */
  if (!display_en) {
    raise(SIGINT);
    return false;
  }
  if (border_en) {
    XEvent event;
    XNextEvent(display, &event);
    switch (event.type) {
      case ClientMessage:
        if (event.xclient.message_type == XInternAtom(display, "WM_PROTOCOLS", 1) &&
            (Atom)event.xclient.data.l[0] == XInternAtom(display, "WM_DELETE_WINDOW", 1)); {
          display_en = false;
          XDestroyWindow(display, win);
          XCloseDisplay(display);
          return false;
        }
      default:
        break;
    }
  }

  /* screen.blit(framebuffer) */
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      int color = framebuffer[i][j];
      XSetForeground(display, gc, color);
      XDrawPoint(display, win, gc, i, j);
    }
  }
  XFlush(display);
  return true;
}

void AvieVideo::tick(int fps) {
  struct timespec sleeptime = {
tv_sec: 0,
        tv_nsec: 1000000000 / fps
  };
  nanosleep(&sleeptime, NULL);
}

#include "AvieWindow.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifdef __gnu_linux__
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
#elif _WIN32
#include <windows.h>
#include <SDL/SDL_syswm.h>
#include <SDL/SDL_image.h>
#endif

AvieWindow::AvieWindow(int w, int h, int xpos, int ypos, bool border) {
  width = w;
  height = h;
  x = xpos;
  y = ypos;

#ifdef __gnu_linux__
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

#elif _WIN32
  /* initialize SDL */
  SDL_Init(SDL_INIT_EVERYTHING);

  /* create display on the windows env */
  screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);

  /* set the window to be transparent */
  HWND hwnd;
  SDL_SysWMinfo info;
  SDL_VERSION(&info.version);
  if (SDL_GetWMInfo(&info))
    hwnd = info.window;
  SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
  SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), (255 * 70) / 100, LWA_COLORKEY);

#endif
  display_en = true;

  /* create framebuffer */
  framebuffer = (uint32_t **)malloc(sizeof(uint32_t *) * width);
  for (int i = 0; i < width; i++)
    framebuffer[i] = (uint32_t *)malloc(sizeof(uint32_t) * height);
  reset();
}

AvieWindow::~AvieWindow() {
  if (display_en) {
    display_en = false;
#ifdef __gnu_linux__
    XDestroyWindow(display, win);
    XCloseDisplay(display);
#elif _WIN32
    SDL_FreeSurface(screen);
    SDL_Quit();
#endif
  }
  for (int i = 0; i < width; i++)
    free(framebuffer[i]);
  free(framebuffer);
}

bool AvieWindow::flush() {
  /* if (game.exit_signal) quit() */
  if (!display_en) {
    raise(SIGABRT);
    return false;
  }
  if (border_en) {
#ifdef __gnu_linux__
    XEvent event;
    XNextEvent(display, &event);
    switch (event.type) {
      case ClientMessage:
        if (event.xclient.message_type == XInternAtom(display, "WM_PROTOCOLS", 1) &&
            (Atom)event.xclient.data.l[0] == XInternAtom(display, "WM_DELETE_WINDOW", 1)) {
          display_en = false;
          XDestroyWindow(display, win);
          XCloseDisplay(display);
          return false;
        }
      default:
        break;
    }
#elif _WIN32
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (events.type) {
        case SDL_QUIT:
          display_en = false;
          SDL_FreeSurface(screen);
          SDL_Quit();
          return false;
        default:
          break;
      }
    }
#endif
  }

  /* screen.blit(framebuffer) */
#ifdef _WIN32
  SDL_LockSurface(screen);
  uint32_t *pixels = (uint32_t *)screen->pixels;
#endif
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      int color = framebuffer[i][j];
      if (ALPHA(color) == 0x00)
        continue;
#ifdef __gnu_linux__
      XSetForeground(display, gc, color);
      XDrawPoint(display, win, gc, i, j);
#elif _WIN32
      uint32_t c = SDL_MapRGBA(screen->format,
          RED(color), GREEN(color), BLUE(color), ALPHA(color));
      pixels[j * surface->w + i] = c;
#endif
    }
  }
#ifdef __gnu_linux__
  XFlush(display);
#elif _WIN32
  SDL_UnlockSurface(screen);
  SDL_Flip(screen);
#endif
  reset();
  return true;
}

void AvieWindow::tick(int fps) {
  struct timespec sleeptime = { tv_sec: 0, tv_nsec: 1000000000 / fps };
  nanosleep(&sleeptime, NULL);
}

void AvieWindow::reset() {
  for (int i = 0; i < width; i++)
    memset(framebuffer[i], 0, sizeof(uint32_t) * height);
}

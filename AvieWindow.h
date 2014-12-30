#ifndef AvieWindow_h
#define AvieWindow_h

#include <stdint.h>
#include "AvieCommon.h"

// Window type depends on platform
#ifdef __gnu_linux__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#elif _WIN32
#include <SDL/SDL.h>
#endif

class AvieWindow {
  public:
    // framebuffer[x][y] returns a color
    uint32_t **framebuffer;
    int x;
    int y;
    int width;
    int height;

    AvieWindow(int w, int h,
        int xpos = 0, int ypos = 0,
        bool border = false);
    ~AvieWindow();
    bool flush();
    void tick(int fps);
    void reset(); // called from within flush

  private:
    bool display_en;
    bool border_en;
#ifdef __gnu_linux__
    Display *display;
    Window win;
    GC gc;
#elif _WIN32
    SDL_Surface *screen;
#endif
};

#endif

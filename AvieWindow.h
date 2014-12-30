#ifndef AvieWindow_h
#define AvieWindow_h

#include <stdint.h>

// Window type depends on platform
#ifdef __gnu_linux__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#elif _WIN32
#include <SDL/SDL.h>
#endif

// Create colors from rgb or rgba values
#define RGB(r, g, b) \
  ((0xFF000000) + \
   (((r) & 0x000000FF) << 16) + \
   (((g) & 0x000000FF) << 8) + \
   (((b) & 0x000000FF)))

#define RGBA(r, g, b, a) \
  ((((a) & 0x000000FF) << 24) + \
   (((r) & 0x000000FF) << 16) + \
   (((g) & 0x000000FF) << 8) + \
   (((b) & 0x000000FF)))

// Extract rgba values from colors
#define RED(c)   ((c & 0x00FF0000) >> 16)
#define GREEN(c) ((c & 0x0000FF00) >> 8)
#define BLUE(c)  ((c & 0x000000FF) >> 0)
#define ALPHA(c) ((c & 0xFF000000) >> 24)

class AvieWindow {
  public:
    // framebuffer[x][y] returns a color
    uint32_t **framebuffer;
    int x;
    int y;
    int w;
    int h;

    AvieWindow(int width,
              int height,
              int xpos = 0,
              int ypos = 0,
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

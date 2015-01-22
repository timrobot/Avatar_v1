#ifndef AvieWindow_h
#define AvieWindow_h

#include <stdint.h>
#include "AvieCommon.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

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
    void blit(uint32_t **frame, int width, int height);
    bool flush();
    void tick(int fps);
    void reset();

  private:
    bool display_en;
    bool border_en;
    Display *display;
    Window win;
    GC gc;
    uint8_t **blitmask; // for blitting faster?
};

#endif

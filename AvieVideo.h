#ifndef AvieVideo_h
#define AvieVideo_h

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdint.h>

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

typedef union {
  uint32_t bit32;
  struct {
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
  };
} ARGB_t;

class AvieVideo {
  public:
    uint32_t **framebuffer; /* data[x][y] returns a color { A, R, G, B } */
    int x;
    int y;
    int w;
    int h;

    AvieVideo(int width, int height, bool border = false);
    ~AvieVideo();
    bool flush();
    void tick(int fps);

  private:
    bool display_en;
    bool border_en;
    Display *display;
    Window win;
    GC gc;
};

#endif

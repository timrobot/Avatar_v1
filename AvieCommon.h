#ifndef AvieDefines_h
#define AvieDefines_h

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

const int debug = 0;

#endif

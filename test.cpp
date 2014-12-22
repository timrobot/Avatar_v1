#include <signal.h>
#include <stdio.h>
#include "AvieVideo.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

static int exit_signal;
void sigint(int signo);
void reset(AvieVideo *window);
void blit(uint32_t **screen, Mat& image);

int main(int argc, char **argv) {

  // example: read in images and display them
  if (argc < 2) { fprintf(stderr, "usage: %s [images]\n", argv[0]); return 1; }
  int numImages = argc - 1;
  Mat images[numImages];
  for (int i = 0; i < numImages; i++)
    images[i] = imread(argv[i + 1]);
  signal(SIGINT, sigint);

  // step 1: create screen
  AvieVideo *window = new AvieVideo(300, 300);
  uint32_t **screen = window->framebuffer;

  // step 2: draw stuff
  int pos = 0;
  while (!exit_signal) {
    reset(window);                      // reset screen to transparent
    blit(screen, images[pos]);          // draw image to screen
    pos = (pos + 1) % numImages;

    window->flush();                    // update window
    window->tick(30);                   // wait (30 fps)
  }

  // step 3: clean up when done
  delete(window);
  return 0;
}

void sigint(int signo) {
  exit_signal = 1;
}

void reset(AvieVideo *window) {
  for (int i = 0; i < window->w; i++)
    for (int j = 0; j < window->h; j++)
      window->framebuffer[i][j] = RGBA(0, 0, 0, 0);
}

void blit(uint32_t **screen, Mat& image) {
  // show blue only when black
  for (int i = 0; i < image.cols; i++)
    for (int j = 0; j < image.rows; j++)
      if (image.at<Vec3b>(j, i) == Vec3b(0, 0, 0))
        screen[i][j] = RGB(75, 150, 255);
}

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include "AvieWindow.h"
#include "AvieAudio.h"

using namespace cv;
using namespace std;
void blit(uint32_t **screen, Mat& image);

// example: read in images and display them
int main(int argc, char **argv) {

  // argument check
  if (argc < 2) {
    fprintf(stderr, "usage: %s [images]\n", argv[0]);
    return 1;
  }

  // hack: say im starting
  AvieAudio speaker;
  speaker.say("I am now starting the program");

  // read in images, store on queue
  vector<Mat> images;
  for (int i = 1; i < argc; i++)
    images.push_back(imread(argv[i]));


  // create window, get screen from window
  AvieWindow window(300, 300);
  uint32_t **screen = window.framebuffer;


  // draw images
  while (1) {
    for (int i = 0; i < images.size(); i++) {
      blit(screen, images[i]);    // copy image to screen
      window.flush();             // actually show the screen
      window.tick(30);            // wait (30 fps)
    }
  }

  return 0;
}



void blit(uint32_t **screen, Mat& image) {
  // show blue only when black
  for (int i = 0; i < image.cols; i++)
    for (int j = 0; j < image.rows; j++)
      if (image.at<Vec3b>(j, i) == Vec3b(0, 0, 0))
        screen[i][j] = RGB(75, 150, 255);
}

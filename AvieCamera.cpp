#include "AvieCamera.h"

using namespace std;

AvieCamera::AvieCamera(int camid) {
  camera.open(camid);
  width = camera.get(CV_CAP_PROP_FRAME_WIDTH);
  height = camera.get(CV_CAP_PROP_FRAME_HEIGHT);
  framebuffer = (uint32_t **)malloc(sizeof(uint32_t *) * width);
  for (int i = 0; i < width; i++) {
    framebuffer[i] = (uint32_t *)malloc(sizeof(uint32_t) * height);
    memset(framebuffer[i], 0, sizeof(uint32_t) * height);
  }
}

AvieCamera::~AvieCamera() {
  if (camera.isOpened())
    camera.release();
  for (int i = 0; i < width; i++)
    free(framebuffer[i]);
  free(framebuffer);
}

uint32_t **AvieCamera::getFrame() {
  if (camera.isOpened()) {
    camera >> frame;
    for (int i = 0; i < width; i++)
      for (int j = 0; j < height; j++)
        framebuffer[i][j] = RGB(
            frame.at<cv::Vec3b>(j, i)[2],
            frame.at<cv::Vec3b>(j, i)[1],
            frame.at<cv::Vec3b>(j, i)[0]);
  }
  return framebuffer;
}

#ifndef AvieCamera_h
#define AvieCamera_h

#include <stdint.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "AvieCommon.h"

class AvieCamera {
  public:
    AvieCamera(int camid = 0);
    ~AvieCamera();

    uint32_t **getFrame();
    int width;
    int height;

  private:
    uint32_t **framebuffer;
    cv::Mat frame;
    cv::VideoCapture camera;
};

#endif

#ifndef AvieCamera_h
#define AvieCamera_h

#include <stdint.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class AvieCamera() {
  public:
    AvieCamera(int camid = 0);
    ~AvieCamera();

    uint32_t **getFrame();

  private:
    uint32_t **frame;
    cv::VideoCapture camera;
};

#endif

#include "AvieCamera.h"

using namespace std;

AvieCamera::AvieCamera(int camid) {
  camera.open(camid);
  frame = (uint32_t **)malloc(sizeof(uint32_t *) * camera.width);
  for (int i = 0; i < camera.width; i++) {
    frame[i] = (uint32_t *)malloc(sizeof(uint32_t) * camera.height);
    memset(frame[i], 0, sizeof(uint32_t) * camera.height);
  }
}

AvieCamera::~AvieCamera() {
  if (camera.opened())
    camera.close();
  for (int i = 0; i < camera.width; i++)
    free(frame[i]);
  free(frame);
}

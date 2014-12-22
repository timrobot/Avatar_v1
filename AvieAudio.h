#ifndef AvieAudio_h
#define AvieAudio_h

#include <string>

class AvieAudio {
  public:
    AvieAudio(std::string voice_name = "");
    void say(std::string message);
    std::string listen(void);
};

#endif

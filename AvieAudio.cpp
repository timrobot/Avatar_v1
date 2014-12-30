#include "AvieAudio.h"
#include <festival.h>
#include <vector>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

AvieAudio::AvieAudio(std::string voice_name) {
  std::vector<std::string> voices;
  std::vector<std::string> names;

  // kal
  voices.push_back("(voice_kal_diphone)");
  names.push_back("kal");

  // clb (CMU arctic)
  voices.push_back("(voice_cmu_us_clb_arctic_clunits)");
  names.push_back("clb");

  festival_initialize(1, 420000);
  int i;
  for (i = 0; i < MIN(voices.size(), names.size()); i++)
    if (names[i] == voice_name)
      break;
  if (i == MIN(voices.size(), names.size()))
    i = 0;
  festival_eval_command(voices[i].c_str());
}

void AvieAudio::say(std::string message) {
  festival_say_text(message.c_str());
  festival_wait_for_spooler();
}

std::string AvieAudio::listen(void) {
  return NULL;
}

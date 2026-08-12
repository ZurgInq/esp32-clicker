#include "loopInterval.h"

LoopInterval::LoopInterval(unsigned long delay) {
  this->delay = delay;
}

unsigned long LoopInterval::loopInterval(unsigned long now) {
  unsigned long tPassed = now - last;

  if (tPassed >= delay) {
    last = now;
    return tPassed;
  }

  return 0;
};

#include "Core.h"
#include <iostream>

bool PROG::PROG_RUNNING_b = false;
bool PROG::THREADS_RUNNING_b = false;

namespace CJ {
  void progChecker() {
    bool breaker = false;
    while (!breaker) {
      if (!PROG::PROG_RUNNING()) {
        std::cerr << "Program no longer running" << std::endl;
        PROG::set_THREADS_RUNNING(false);
        breaker = true;
      }

      if (!PROG::THREADS_RUNNING()) {
        std::cerr << "Program threads are not running" << std::endl;
      }
    }
  }


  void Core::init() {
    PROG::set_PROG_RUNNING(true);
    PROG::set_THREADS_RUNNING(true);
    std::thread progChecker_t(progChecker);
    progChecker_t.detach();
  }

  void Core::setupVision(Image *image, Camera::Cam *cam) {
    if (PROG::PROG_RUNNING()) {
      Camera::camSetup(image, cam);
    } else {
      std::cerr << "ERROR: Program core must be initialized. Run `CJ::Core::init()`" << std::endl;
    }
  }
}
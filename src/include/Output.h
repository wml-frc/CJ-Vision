#ifndef DISPLAY_H
#define DISPLAY_H
#include "cjHeaders.h"
#include "ImageContainer.h"
#include <thread>

namespace CJ {
  class Output {
    public:
      static void Display(Image *image);
    private:
  };
}

#endif
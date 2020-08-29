#ifndef COLOUR_TRACKING_H
#define COLOUR_TRACKING_H

#include "cjHeaders.h"
#include "ImageContainer.h"

namespace CJ {
  class ColourTracking {
   public:
    static void track(Image *outputImage, Image *inputImage);
  };
}
#endif
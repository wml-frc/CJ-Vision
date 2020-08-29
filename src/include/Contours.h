#ifndef CONTOURS_H
#define CONTOURS_H

#include "cjHeaders.h"

namespace CJ {
  class Contours {
   public:
    /**
     * Detect Edges using canny algorithm
     */
    static void cannyTrack();

    /**
     * Detect Contours in image
     */
    static void detectContours();

    /**
     * Detect edges by thresholding image until point of interest is left
     */
    static void thresholdImage();
  };
}
#endif
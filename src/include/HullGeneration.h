#ifndef HULL_GENERATION_H
#define HULL_GENERATION_H

#include "cjHeaders.h"

namespace CJ {
  class HullGeneration {
    /**
     * Draw Hull around contours
     */
    static void drawHull();

    /**
     * Draw Bounding Box around objects/hull
     */
    static void drawBoundingBox();
  };
}
#endif
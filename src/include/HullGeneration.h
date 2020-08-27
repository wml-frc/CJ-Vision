#ifndef HULL_GENERATION_H
#define HULL_GENERATION_H

#include "cvHeaders.h"

namespace CJ {
  class HullGeneration {
    /**
     * Draw Hull around contours
     */
    void drawHull();

    /**
     * Draw Bounding Box around objects/hull
     */
    void drawBoundingBox();
  };
}
#endif
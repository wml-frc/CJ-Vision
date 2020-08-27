#ifndef COLOUR_TRACKING_H
#define COLOUR_TRACKING_H

#include "cvHeaders.h"

namespace CJ {
  class ColourTracking {
   public:
    /**
     * Filters Out other pixels and leaves green for tracking retro reflective tape.
     */
    void retroTrack();

    /**
     * Use your own colour and specs to track a certain colour
     */
    void customTrack();
  };
}
#endif
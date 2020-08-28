// CJ Vision
#include "ImageContainer.h"
#include "ColourTracking.h"
#include "Camera.h"
#include "Output.h"

namespace CJ {
  class Core {
   public:

    /**
     * Init program.
     */
    static void init();

    /**
    * Sets up the vision library with cam and image settings
    */
    void setupVision(Image *image, Camera::Cam *cam);

   private:

    /**
     * Retro tape tracking values
     */
    const int RETRO_H_MIN = 50;
    const int RETRO_H_MAX = 70;

    const int RETRO_S_MIN = 250;
    const int RETRO_S_MAX = 255;

    const int RETRO_V_MIN = 30;
    const int RETRO_V_MAX = 255;
  };
}
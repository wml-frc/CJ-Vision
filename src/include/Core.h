// CJ Vision
#include "ImageContainer.h"
#include "ColourFilter.h"
#include "Camera.h"
#include "Output.h"

// CJ Network
#include "socket/Network.h"

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
  };
}
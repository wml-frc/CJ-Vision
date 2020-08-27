// #ifndef CAMERA_H
// #define CAMERA_H

#include "cvHeaders.h"
#include "ImageContainer.h"
#include <iostream>

namespace CJ {

  class Camera {
   public:
    struct cameraConfig {
      int CamPort = 0;
      int FPS = 30;
      int ResHeight = 480;
      int ResWidth = 640;
      int Exposure = 100;
      std::string CamName = "Cam";
      bool RetroTrack = false;
    };

    struct Cam {
      cameraConfig config;
      cv::VideoCapture cap;
    };

    /**
     * Set up camera (Size, Exposure & Name)
     */
    static void camSetup(Cam *cam);

    /**
     * Get video mode of camera
     */
    auto getVideoMode();

   private:
    cv::Mat imageReturn();
  };
}
// #endif

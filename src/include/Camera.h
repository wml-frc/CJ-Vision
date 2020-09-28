#ifndef CAMERA_H
#define CAMERA_H

#include "cjHeaders.h"
#include "ImageContainer.h"
#include <iostream>
#include <thread>

namespace CJ {

  class Camera {
   public:
    struct cameraConfig {
      int CamPort = 0;
      int apiID = cv::CAP_ANY;
      int FPS = 30;
      int ResWidth = 640;
      int ResHeight = 480;
      double Exposure = 0.1;
      bool AutoExposure = true;
      std::string CamName = "Cam";
    };

    struct Cam {
      cameraConfig config;
      cv::VideoCapture cap;
    };

    /**
     * Set up camera (Size, Exposure & Name)
     */
    static void camSetup(Image *image, Cam *cam);


   private:
    static void capture(Image *image, Cam *cam);
  };
}
#endif

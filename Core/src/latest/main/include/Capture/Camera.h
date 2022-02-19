#ifndef CAMERA_H
#define CAMERA_H

#include "common_headers.h"
#include "ImageContainer.h"

namespace CJ {

  /**
    * Camera class
    * Wrapper for OpenCV camera, with init, update functions and config structures
    */
  class Camera {
  public:
    ~Camera();

    /**
      * Camera config.
      * FPS, Auto Exposure, width, height etc....
      */
    struct Config {
      int port = 0;
      int apiID = cv::CAP_ANY;
      int fps = 30;
      int resWidth = 640;
      int resHeight = 480;
      bool autoExposure = true;
      bool useAbsoluteExposure = false; // overrides auto exposure (uses linux scripts)
      float exposure = 0.1f;
      std::string name = "Cam";

      float cap_prop_autoExpose = 0.75f; // default value to set camera for auto exposure ON
      float cap_prop_manualExpose = 0.25f; // default value to set camera for auto exposure OFF
    };
    
    Config config;
    cv::VideoCapture cap;


    /**
      * Setup the camera on port with config.
      * Returns 0/1 on success/failure
      */
    int init();

    /**
      * Update the camera class.
      * Capture image from the camera and pass it to the image.
      */
    void capture(Image &image);

    operator cv::VideoCapture() {
      return cap;
    }

    operator Config() {
      return config;
    }

  private:
    bool _trackBar = false;
  };
}

#endif
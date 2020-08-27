// Base Libraries
#include <stdio.h>
#include <iostream>
#include <thread>

// Camera Servers
// #include <cameraserver/CameraServer.h>
// #include <cscore.h>

// CJ Vision
#include "ImageContainer.h"
#include "ColourTracking.h"
#include "Camera.h"

namespace CJ {
  class VisionCore : public Image, public Camera {

   /**
    * Sets up the vision library with cam and image settings
    */
   void setupVision();
  };
}
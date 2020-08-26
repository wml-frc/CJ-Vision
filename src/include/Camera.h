#ifndef CAMERA_H
#define CAMERA_H
// #include <cscore.h>
#include <opencv2/opencv.hpp>
namespace CJ {
  class Camera {
   public:
    /**
     * Set up camera (Size, Exposure & Name)
     */
    // cs::UsbCamera camSetup();

    /**
     * Get video mode of camera
     */
    auto getVideoMode();

   private:
    cv::Mat imageReturn();
  };
}
#endif
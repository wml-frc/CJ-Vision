#include "VisTrack.h"

CJ::VisionTracking vision;

void CJ::VisionOutput::Display(cv::Mat *Image, std::string ImageName) {
  #ifdef __DESKTOP__
  cv::imshow(ImageName, *Image);
  #endif
  vision.Camera.cam.output.PutFrame(*Image);
  cv::waitKey(1);
}
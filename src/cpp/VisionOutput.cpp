#include "VisTrack.h"
#include <cstdarg>

CJ::VisionTracking vision;

void CJ::VisionOutput::Display(std::string ImageName, cv::Mat *Image) {
  #ifdef __DESKTOP__
  cv::imshow(ImageName, *Image);
  #endif
  vision.Camera.cam.output.PutFrame(*Image);
}
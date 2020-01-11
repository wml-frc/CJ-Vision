#include "VisTrack.h"
#include <cstdarg>

CJ::VisionTracking vision;

cv::Mat DisplayImg;

void CJ::VisionOutput::Display(std::string ImageName, cv::Mat *Image) {
  Image->copyTo(DisplayImg);
  #ifdef __DESKTOP__
  cv::imshow(ImageName, DisplayImg);
  #else
  vision.Camera.cam.output.PutFrame(DisplayImg);
  #endif
  std::this_thread::sleep_for(std::chrono::milliseconds(30));
}
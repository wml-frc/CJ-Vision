#include "VisTrack.h"

void CJ::VisionTracking::Display(std::string ImageName, cv::Mat *Image) {
  Image->copyTo(DisplayImg);
  #ifdef __DESKTOP__
  cv::imshow(ImageName, DisplayImg);
  cv::waitKey(30);
  #else
  Camera.cam.output.PutFrame(DisplayImg);
  #endif
}
#ifndef IMAGE_CONTAINER_H
#define IMAGE_CONTAINER_H

#include "cjHeaders.h"

namespace CJ {
  struct Image {
    cv::Mat data;
    std::string name = " ";
    std::vector<std::vector<cv::Point>> contours;
  };
}
#endif
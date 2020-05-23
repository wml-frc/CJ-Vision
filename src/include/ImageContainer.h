#ifndef IMAGE_CONTAINER_H
#define IMAGE_CONTAINER_H
#include <opencv2/opencv.hpp>
#include <vector>
namespace CJ {
  struct Image {
    cv::Mat image;
    std::vector<std::vector<cv::Point>> contours;
  };
}
#endif
#include "ImageProcessing/Contours.h"

namespace CJ {
  void Contours::cannyTrack(Image &input, Image &output, int threshold) {
    cv::Canny(input.data, output.data, threshold, threshold*2);
  }

  void Contours::detectContours(Image &input, Image &output, bool disableDraw) {
    if (input.data.empty()) {
      CJ_CORE_PRINT_ERROR("Can't get input image: " + input.name + ". (Colour Filter)");
      return;
    }

    cv::findContours(input.data, output.contours, output.hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));

    if (!disableDraw) {
      // cv::RNG rng;
      output.data = cv::Mat::zeros(input.data.size(), CV_8UC3);
      for (size_t i = 0; i < output.contours.size(); i++) {
        cv::Scalar colour = cv::Scalar(0, 255, 0);
        cv::drawContours(output.data, output.contours, i, colour);
      }
    }
  }

  void Contours::createContoursTrackbar(int &threshold) {
    #ifndef COPROC
    std::string windowName = "Contour Threshold Editor";
    cv::namedWindow(windowName);
    cv::createTrackbar("Threshold", windowName, &threshold, 255);
    #endif
  }
}
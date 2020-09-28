#include "ColourFilter.h"
#include <thread>

namespace CJ {
  cv::Mat LocalProcessingMAT;
  void filter_thread(Image *inputImage, Image *outputImage, ColourFilter::Options options) {
    while (PROG::PROG_RUNNING()) {
      cv::cvtColor(inputImage->data, LocalProcessingMAT, cv::COLOR_BGR2HSV);
      cv::inRange(LocalProcessingMAT, cv::Scalar(options.HColourLowRange, options.SColourLowRange, options.VColourLowRange), 
                                      cv::Scalar(options.HColourHighRange, options.SColourHighRange, options.VColourHighRange), LocalProcessingMAT);

      if (options.ErosionSize > 0) {
        cv::erode(LocalProcessingMAT, LocalProcessingMAT, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(options.ErosionSize, options.ErosionSize)));
        cv::dilate(LocalProcessingMAT, LocalProcessingMAT, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(options.dilationSize, options.dilationSize)));
      }

      LocalProcessingMAT.copyTo(outputImage->data);
    }
  }

  void ColourFilter::filter(Image *inputImage, Image *outputImage, Options options) {
    while (inputImage->data.empty()) {
      std::cout << "Can't get input image. (filter thread)" << std::endl;
    }
    std::thread filter_t(filter_thread, inputImage, outputImage, options);
    while (outputImage->data.empty()) {
      std::cout << "Can't get output image. (filter thread)" << std::endl;
    }
    filter_t.detach();
  }
}
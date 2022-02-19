#ifndef IMAGE_CONTAINER_H
#define IMAGE_CONTAINER_H

#include "common_headers.h"

namespace CJ {
  struct Image {
    /**
     * Data
     */
    cv::Mat data;
    std::string name = "Image"; // Name

    /**
      * Internal extras
      * (Contours, hull vectors etc...)
      */
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point>> hull;

    /**
      * Operators
      */
    // data
    operator cv::Mat() {
      return data;
    }

    // name
    operator std::string() {
      return name;
    }

    // contours
    operator std::vector<std::vector<cv::Point>>() {
      return contours;
    }

    // hierarchy
    operator std::vector<cv::Vec4i>() {
      return hierarchy;
    }
  };
}

#endif
#pragma once 

#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <iostream>

#include <cameraserver/CameraServer.h>
#include <cscore.h>

namespace CJ {
  struct VisionOutput {

    /**
     * Displays Images passed to function on computer and coprocessor.
     * You can dissable the coprocessors display by `DisplayOnCoProcessor false` (This won't dissable the images being sent to network tables)
    */
    void Display(cv::Mat *Image, std::string ImageName);
  };
} 
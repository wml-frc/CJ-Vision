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
    struct ImageOutput {
    
      /**
       * Outputs the Image to a window when running the program locally
       */ 
      void DesktopOutput(cv::Mat ProcessedImg);


      /** 
       * Outputs the Image to CS when running on Sub Processor
       */
      void RobotOutput(cv::Mat ProcessedImg);
    };
    ImageOutput imageOutput;
  };
}
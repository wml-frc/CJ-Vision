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

// Tracking Libraires
#include "VisionCameras.h"
#include "VisionProcessingType.h"
#include "VisionOutput.h"

#include <cameraserver/CameraServer.h>
#include <cscore.h>

namespace CJ {
  class VisionTracking {
    public:
      cv::Mat ImageSrc;
      cv::Mat imgTracking;
      cs::UsbCamera cam;
      
      /** 
       * Sets up vision using OpenCV & Camera Servers
       */
      cv::Mat SetupVision(int CamPort, int FPS, int ResHeight, int ResWidth, int Exposure, std::string CamName, bool RetroTrack);


      /**
       * Track using retro reflective tape, Using low exposure and Green pixle filtering
       * Using the defaults for the colour spectrum and exposure settings.
       */
      cv::Mat RetroTrack(cv::Mat Img, int ErosionSize, int DialationSize);

      /**
       * Track using your own adjusted settings for the colour spectrum and exposure
       */
      cv::Mat CustomTrack(cv::Mat Img, int HSVColourLowRange, int HSVColourHighRange, int ValueColourLowRange, int ValueColourHighRange, int CamExposure, int ErosionSize, int DialationSize, cs::UsbCamera cam);

      // Instances
      VisionCamera Camera;
      VisionOutput Output;
      VisionProcessing Processing;
  };
}
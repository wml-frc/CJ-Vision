#pragma once

#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <opencv2/core/types.hpp>

// Base Libraries
#include <stdio.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// Tracking Libraires
#include "VisionCameras.h"
#include "VisionProcessingType.h"

#include <cameraserver/CameraServer.h>
#include <cscore.h>

namespace CJ {
  class VisionTracking {
    public:
      cv::Mat DisplayImg;
      cs::UsbCamera cam;
      cv::Mat *MatPtr;
      bool DisplayOnCoProcessor = true;
      bool ImageReturnGood = false;

      /** 
       * Sets up vision using OpenCV & Camera Servers
       */
      void SetupVision(cv::Mat *ImageImage , int CamPort, int FPS, int ResHeight, int ResWidth, int Exposure, std::string CamName, bool RetroTrack);


      /**
       * Track using retro reflective tape, Using low exposure and Green pixle filtering
       * Using the defaults for the colour spectrum and exposure settings.
       */
      void RetroTrack(cv::Mat *OutputImage, cv::Mat *InputImage, int ErosionSize, int DialationSize);


      /**
       * Track using your own adjusted settings for the colour spectrum using HSV
       */
      void CustomTrack(cv::Mat *OutputImage, cv::Mat *InputImage, int HSVColourLowRange, int HSVColourHighRange, int ValueColourLowRange, int ValueColourHighRange, int ErosionSize, int DialationSize);

      /**
       * Displays Images passed to function on computer and coprocessor.
       * You can dissable the coprocessors display by `DisplayOnCoProcessor false` (This won't dissable the images being sent to network tables)
       */
      void Display(std::string ImageName, cv::Mat *Image);

      // Instances
      VisionCamera Camera;
      VisionProcessing Processing;
  };
}
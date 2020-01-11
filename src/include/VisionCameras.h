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
  struct VisionCamera {

    struct Camera {

      
      cs::CvSink sink{"USB"};
      cs::CvSource output;
      cs::VideoMode video_modeStartup;
      cv::Mat *ImgTracking;
      bool CamStartUp = true;

      /** 
       * Sets up the camera up using a Port, Framerate, Height, Width, Exposure level and the Name of the Camera e.g Front
       */
      cs::UsbCamera CamSetup(int Port, double FPS, int ResHeight, int ResWidth, int Exposure, std::string CamName, bool RetroTrack);

      /**
       * Get's the Video Mode from the camera
       */
      auto VideoMode(cs::UsbCamera cam, std::string camName);


      /**
       * Returns the image as a Mat from the Camera
       */
      cv::Mat ImageReturn(cs::UsbCamera cam, std::string camName);
    };
    Camera cam;

    struct Kinect {
      //@todo
    };
    Kinect kinect;
  };
}
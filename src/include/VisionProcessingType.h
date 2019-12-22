#pragma once 

#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <thread>
#include <mutex>
#include <stdio.h>
#include <iostream>

#include <cameraserver/CameraServer.h>
#include <cscore.h>

namespace CJ {

  struct VisionProcessing {

    struct VisionEdgeDetection {

      /**
       * Using OpenCV's Canny operation to detect edges, Then placing those detections ontop of image.
       */
      void CannyTrack(cv::Mat *Image, cv::Mat *Canny_Output, int Threshold);

      /**
       * Detect contours using point to point algorithms, And then stored within vectors
       */
      void ContourDetect(cv::Mat *Image, cv::Mat *OutputImage, int Threshold, int MaxVal);

      /**
       * Detect Edges by thresholding the image.
       */
      void ThresholdImage(cv::Mat *Image, int ThreshMin, int ThreshMax);

      //@Todo, Allow a different edge track for on Rio services
    };
    VisionEdgeDetection visionEdgeDetection;

    struct VisionHullGeneration {

      /**
       * Draws a shell/hull around the object, usefull for detecting centroids of objects
       * Or using the shell to calculate object specific outputs. e.g, angle of rectangle or size of circle.
       */
      void GetHull(cv::Mat *Image);

      /**
       * Draws a bounding box around an object
       */
      void BoundingBox(cv::Mat *Image);
    };
    VisionHullGeneration visionHullGeneration;

    struct VisionDetectionType {

      /**
       * Gives the centroid figure in pixle x,y
       */
      void CentroidDetect(cv::Mat *Image);

      /**
       * Calculates the centroid in pixle x,y and outputs angle & distance of rectangle 
       */
      void RectangleDetect(cv::Mat *Image);

      /**
       * Calculates the centroid in pixle x,y and outputs the distance of the circle 
       */
      void CircleDetect(cv::Mat *Image);
    };
    VisionDetectionType visionDetectionType;
  };
}
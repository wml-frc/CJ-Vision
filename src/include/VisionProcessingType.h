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

  struct VisionProcessing {

    struct VisionEdgeDetection {

      /**
       * Using OpenCV's Canny operation to detect edges, Then placing those detections ontop of image.
       */
      void CannyTrack(cv::Mat img, int Threshold);

      /**
       * Detect contours using point to point algorithms, And store within vectors
       */
      void ContourDetect(cv::Mat img);

      //@Todo, Allow a different edge track for on Rio services
    };
    VisionEdgeDetection visionEdgeDetection;

    struct VisionHullGeneration {

      /**
       * Draws a shell/hull around the object, usefull for detecting centroids of objects
       * Or using the shell to calculate object specific outputs. e.g, angle of rectangle or size of circle.
       */
      void GetHull(cv::Mat img);

      /**
       * Draws a bounding box around an object
       */
      void BoundingBox(cv::Mat img);
    };
    VisionHullGeneration visionHullGeneration;

    struct VisionDetectionType {

      /**
       * Gives the centroid figure in pixle x,y
       */
      void CentroidDetect(cv::Mat img);

      /**
       * Calculates the centroid in pixle x,y and outputs angle & distance of rectangle 
       */
      void RectangleDetect(cv::Mat img);

      /**
       * Calculates the centroid in pixle x,y and outputs the distance of the circle 
       */
      void CircleDetect(cv::Mat img);
    };
    VisionDetectionType visionDetectionType;

  };
}
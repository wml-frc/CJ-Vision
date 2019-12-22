#include "VisTrack.h"

CJ::VisionTracking visionProcessing;

cv::Mat LocalProcessImage_Contours;
cv::Mat LocalProcessImage_Hull;

cv::RNG rng(12345);

void CannyTrackThread(cv::Mat *InputImage, cv::Mat *Canny_Output, int Threshold) {
  while (true) {
    cv::Canny(*InputImage, *Canny_Output, Threshold, Threshold * 2);
  }
}
void CJ::VisionProcessing::VisionEdgeDetection::CannyTrack(cv::Mat *Image, cv::Mat *Canny_Output, int Threshold) {
  std::thread CannyThread(CannyTrackThread, Image, Canny_Output, Threshold);
  CannyThread.detach();
}

void ThreshImageThreadFunction(cv::Mat *Image, int ThreshMin, int ThreshMax) {
  while (true) {
    cv::threshold(*Image, *Image, ThreshMin, ThreshMax, cv::THRESH_BINARY);
  }
}
void CJ::VisionProcessing::VisionEdgeDetection::ThresholdImage(cv::Mat *Image, int ThreshMin, int ThreshMax) {
  std::thread ThresholdImageThread(ThreshImageThreadFunction, Image, ThreshMin, ThreshMax);
  ThresholdImageThread.detach();
}

void ContourDetectThread(cv::Mat *Image, std::vector<std::vector<cv::Point> > *Contours) {
  
  while (true) {
    cv::findContours(*Image, *Contours, cv::RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    std::vector<std::vector<cv::Point> > contours;
    std::vector<std::vector<cv::Point> > contours_poly(contours.size());
    for (size_t i = 0; i < contours.size(); i++) {
      cv::approxPolyDP(contours[i], contours_poly[i], 3, true);
    }
    for( size_t i = 0; i< contours.size(); i++ ) {
      cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
      drawContours( *Image, contours_poly, (int)i, color );
    }
  }
}
void CJ::VisionProcessing::VisionEdgeDetection::ContourDetect(cv::Mat *Image, std::vector<std::vector<cv::Point> > *contours) {
  std::thread ContourThread(ContourDetectThread, Image, contours);
  ContourThread.detach();
}

void CJ::VisionProcessing::VisionHullGeneration::GetHull(cv::Mat *Image) {

}

void BoundingBoxThread(cv::Mat *Image, cv::Mat *OutputImage) {
  while (true) {
    std::vector<std::vector<cv::Point> > contours;
    //contours = *Contours;
    cv::findContours(*Image, contours, cv::RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    std::vector<std::vector<cv::Point> > contours_poly(contours.size());
    std::vector<cv::Rect> boundRect(contours.size());
    std::vector<cv::Point2f> centers(contours.size());
    std::vector<float> radius( contours.size());

    for (size_t i = 0; i < contours.size(); i++) {
      cv::approxPolyDP(contours[i], contours_poly[i], 3, true);
      boundRect[i] = cv::boundingRect(contours_poly[i]);
    }
    *OutputImage = cv::Mat::zeros(Image->size(), CV_8UC3);
    //*OutputImage = Image->clone();
    for( size_t i = 0; i< contours.size(); i++ ) {
      cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
      //drawContours( *OutputImage, contours_poly, (int)i, color );
      rectangle( *OutputImage, boundRect[i].tl(), boundRect[i].br(), color, 2 );
      //circle( *OutputImage, centers[i], (int)radius[i], color, 2 );
    }
  }
}
void CJ::VisionProcessing::VisionHullGeneration::BoundingBox(cv::Mat *Image, cv::Mat *OutputImage) {
  std::thread BoundingThread(BoundingBoxThread, Image, OutputImage);
  BoundingThread.detach();
}

void CJ::VisionProcessing::VisionDetectionType::CentroidDetect(cv::Mat *Image) {

}

void CJ::VisionProcessing::VisionDetectionType::RectangleDetect(cv::Mat *Image) {

}

void CJ::VisionProcessing::VisionDetectionType::CircleDetect(cv::Mat *Image) {

}
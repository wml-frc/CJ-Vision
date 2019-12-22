#include "VisTrack.h"

CJ::VisionTracking visionProcessing;

cv::Mat LocalProcessImage_Contours;
cv::Mat LocalProcessImage_Hull;

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

void ContourDetectThread(cv::Mat *Image, cv::Mat *OutputImage, int Threshold, int MaxVal) {
  cv::RNG rng(12345);
  std::vector<std::vector<cv::Point> > contours;
  
  while (true) {
    cv::findContours(*Image, contours, cv::RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    std::vector<std::vector<cv::Point> > contours_poly(contours.size());
    std::vector<cv::Rect> boundRect(contours.size());
    std::vector<cv::Point2f> centers(contours.size());
    std::vector<float> radius( contours.size());
    for (size_t i = 0; i < contours.size(); i++) {
      cv::approxPolyDP(contours[i], contours_poly[i], 3, true);
      boundRect[i] = cv::boundingRect(contours_poly[i]);
      cv::minEnclosingCircle(contours_poly[i], centers[i], radius[i]);
    }
    *OutputImage = cv::Mat::zeros(Image->size(), CV_8UC3);
    for( size_t i = 0; i< contours.size(); i++ )
    {
        cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        drawContours( *OutputImage, contours_poly, (int)i, color );
        rectangle( *OutputImage, boundRect[i].tl(), boundRect[i].br(), color, 2 );
        circle( *OutputImage, centers[i], (int)radius[i], color, 2 );
    }
  }
}
void CJ::VisionProcessing::VisionEdgeDetection::ContourDetect(cv::Mat *Image, cv::Mat *OutputImage, int Threshold, int MaxVal) {
  std::thread ContourThread(ContourDetectThread, Image, OutputImage, Threshold, MaxVal);
  ContourThread.detach();
}

void CJ::VisionProcessing::VisionHullGeneration::GetHull(cv::Mat *Image) {

}

void CJ::VisionProcessing::VisionHullGeneration::BoundingBox(cv::Mat *Image) {

}

void CJ::VisionProcessing::VisionDetectionType::CentroidDetect(cv::Mat *Image) {

}

void CJ::VisionProcessing::VisionDetectionType::RectangleDetect(cv::Mat *Image) {

}

void CJ::VisionProcessing::VisionDetectionType::CircleDetect(cv::Mat *Image) {

}
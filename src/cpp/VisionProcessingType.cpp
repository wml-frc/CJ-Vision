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


void ContourDetectThread(cv::Mat *Image, int Threshold, int MaxVal) {
  cv::RNG rng(12345);
  while (true) {
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::threshold(*Image, *Image, Threshold, MaxVal, cv::THRESH_BINARY);
    cv::findContours(*Image, contours, hierarchy, cv::RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    // draw contours onto image
    *Image = cv::Mat::zeros(LocalProcessImage_Contours.size(), CV_8UC3);
    for (int i = 0; i < contours.size(); i++) {
      cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
      cv::drawContours(*Image, contours, i, color, 2, 8, hierarchy, 0, cv::Point());
    }
  }
}
void CJ::VisionProcessing::VisionEdgeDetection::ContourDetect(cv::Mat *Image, int Threshold, int MaxVal) {
  std::thread ContourThread(ContourDetectThread, Image, Threshold, MaxVal);
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
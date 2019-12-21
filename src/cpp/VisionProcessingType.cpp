#include "VisTrack.h"

CJ::VisionTracking visionProcessing;

void CannyTrackThread(cv::Mat, int Threshold) {
  //@todo
}
void CJ::VisionProcessing::VisionEdgeDetection::CannyTrack(cv::Mat Image, int Threshold) {
  std::thread CannyThread(CannyTrackThread, Image, Threshold);
  CannyThread.detach();
}

void CJ::VisionProcessing::VisionEdgeDetection::ContourDetect(cv::Mat img) {

}

void CJ::VisionProcessing::VisionHullGeneration::GetHull(cv::Mat img) {

}

void CJ::VisionProcessing::VisionHullGeneration::BoundingBox(cv::Mat img) {

}

void CJ::VisionProcessing::VisionDetectionType::CentroidDetect(cv::Mat img) {

}

void CJ::VisionProcessing::VisionDetectionType::RectangleDetect(cv::Mat img) {

}

void CJ::VisionProcessing::VisionDetectionType::CircleDetect(cv::Mat img) {

}
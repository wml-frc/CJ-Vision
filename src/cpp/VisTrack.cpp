#include "VisTrack.h"

const int RETRO_HSV_MIN = 35;
const int RETRO_HSV_MAX = 78;

const int RETRO_VALUE_MIN = 100;
const int RETRO_VALUE_MAX = 255;

cv::Mat CJ::VisionTracking::SetupVision(int CamPort, int FPS, int ResHeight, int ResWidth, int Exposure, std::string Name, bool RetroTrack) {
  if (RetroTrack == true){ Exposure = -100; }
  cam = Camera.cam.CamSetup(CamPort, FPS, ResHeight, ResWidth, Exposure, Name);

  ImageSrc = Camera.cam.ImageReturn(cam, Name);

  return ImageSrc;
}

cv::Mat CJ::VisionTracking::RetroTrack(cv::Mat Img, int ErosionSize, int DialationSize) {
  if (Camera.cam.sink.GrabFrame(Img) != 0) {
    cv::cvtColor(Img, imgTracking, cv::COLOR_BGR2HSV); // Uses HSV Spectrum

    // Keeps Only green pixles
    cv::inRange(imgTracking, cv::Scalar(RETRO_HSV_MIN, RETRO_VALUE_MIN, RETRO_VALUE_MIN), cv::Scalar(RETRO_HSV_MAX, RETRO_VALUE_MAX, RETRO_VALUE_MAX), imgTracking);

    // Removes pixles at a certain size, And dilates the image to get rid of gaps
    if (ErosionSize > 0) {
      cv::erode(imgTracking, imgTracking, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(ErosionSize, ErosionSize)));
      cv::dilate(imgTracking, imgTracking, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(DialationSize, DialationSize)));
    }
  } else {
    std::cout << "Error Getting Image" << std::endl;
  }

  return imgTracking;
}

cv::Mat CJ::VisionTracking::CustomTrack(cv::Mat Img, int HSVColourLowRange, int HSVColourHighRange, int ValueColourLowRange, int ValueColourHighRange, int CamExposure, int ErosionSize, int DialationSize, cs::UsbCamera cam) {
  if (Camera.cam.sink.GrabFrame(Img) != 0) {
    cv::cvtColor(Img, imgTracking, cv::COLOR_BGR2HSV); // Uses HSV Spectrum

    // Keeps Only green pixles
    cv::inRange(imgTracking, cv::Scalar(HSVColourLowRange, ValueColourLowRange, ValueColourLowRange), cv::Scalar(HSVColourHighRange, ValueColourHighRange, ValueColourHighRange), imgTracking);
    
    // Removes pixles at a certain size, And dilates the image to get rid of gaps
    if (ErosionSize > 0) {
      cv::erode(imgTracking, imgTracking, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(ErosionSize, ErosionSize)));
      cv::dilate(imgTracking, imgTracking, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(DialationSize, DialationSize)));
    }
  } else {
    std::cout << "Error Getting Image" << std::endl;
  }

  return imgTracking;
}
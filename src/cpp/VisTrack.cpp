#include "VisTrack.h"

const int RETRO_HSV_MIN = 35;
const int RETRO_HSV_MAX = 78;

const int RETRO_VALUE_MIN = 100;
const int RETRO_VALUE_MAX = 255;

CJ::VisionTracking vision;

void CJ::VisionTracking::SetupVision(cv::Mat *ImageSrc, int CamPort, int FPS, int ResHeight, int ResWidth, int Exposure, std::string Name, bool RetroTrack) {
  if (RetroTrack == true){ Exposure = -100; }
  cam = Camera.cam.CamSetup(CamPort, FPS, ResHeight, ResWidth, Exposure, Name);

  *ImageSrc = Camera.cam.ImageReturn(cam, Name);
}

void RetroTrackThread(cv::Mat *OutputImage, cv::Mat InputImage, int ErosionSize, int DialationSize) {
  do {
    cv::cvtColor(InputImage, *OutputImage, cv::COLOR_BGR2HSV); // Uses HSV Spectrum

    // Keeps Only green pixles
    cv::inRange(*OutputImage, cv::Scalar(RETRO_HSV_MIN, RETRO_VALUE_MIN, RETRO_VALUE_MIN), cv::Scalar(RETRO_HSV_MAX, RETRO_VALUE_MAX, RETRO_VALUE_MAX), *OutputImage);

    // Removes pixles at a certain size, And dilates the image to get rid of gaps
    if (ErosionSize > 0) {
      cv::erode(*OutputImage, *OutputImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(ErosionSize, ErosionSize)));
      cv::dilate(*OutputImage, *OutputImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(DialationSize, DialationSize)));
    }
  } while (true);
}

void CJ::VisionTracking::RetroTrack(cv::Mat *OutputImage, cv::Mat InputImage, int ErosionSize, int DialationSize) {
  if (Camera.cam.sink.GrabFrame(InputImage) != 0) {
    std::thread RetroThread(RetroTrackThread, OutputImage, InputImage, ErosionSize, DialationSize);
  } else {
    std::cout << "Error Getting Image (Function Retro Track)" << std::endl;
  }
}


void CJ::VisionTracking::CustomTrack(cv::Mat *OutputImage, cv::Mat InputImage, int HSVColourLowRange, int HSVColourHighRange, int ValueColourLowRange, int ValueColourHighRange, int CamExposure, int ErosionSize, int DialationSize, cs::UsbCamera cam) {
  if (Camera.cam.sink.GrabFrame(InputImage) != 0) {
    cv::cvtColor(InputImage, *OutputImage, cv::COLOR_BGR2HSV); // Uses HSV Spectrum

    // Keeps Only green pixles
    cv::inRange(*OutputImage, cv::Scalar(HSVColourLowRange, ValueColourLowRange, ValueColourLowRange), cv::Scalar(HSVColourHighRange, ValueColourHighRange, ValueColourHighRange), *OutputImage);
    
    // Removes pixles at a certain size, And dilates the image to get rid of gaps
    if (ErosionSize > 0) {
      cv::erode(*OutputImage, *OutputImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(ErosionSize, ErosionSize)));
      cv::dilate(*OutputImage, *OutputImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(DialationSize, DialationSize)));
    }
  } else {
    std::cout << "Error Getting Image" << std::endl;
  }
}
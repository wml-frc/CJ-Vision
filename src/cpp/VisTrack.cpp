#include "VisTrack.h"

const int RETRO_HSV_MIN = 35;
const int RETRO_HSV_MAX = 78;

const int RETRO_VALUE_MIN = 100;
const int RETRO_VALUE_MAX = 255;

CJ::VisionTracking visionVisTrack;
cv::Mat LocalProcessImage;
bool IsDisplayable = false;

void CJ::VisionTracking::SetupVision(cv::Mat *ImageSrc, int CamPort, int FPS, int ResHeight, int ResWidth, int Exposure, std::string Name, bool RetroTrack) {
  if (RetroTrack == true){ Exposure = -100; }
  cam = Camera.cam.CamSetup(CamPort, FPS, ResHeight, ResWidth, Exposure, Name);

  *ImageSrc = Camera.cam.ImageReturn(cam, Name);
}

void RetroTrackThread(cv::Mat *OutputImage, cv::Mat *InputImage, int ErosionSize, int DialationSize) {
  while (true) {
    cv::cvtColor(*InputImage, LocalProcessImage, cv::COLOR_BGR2HSV); // Uses HSV Spectrum

    // Keeps Only green pixles
    cv::inRange(LocalProcessImage, cv::Scalar(RETRO_HSV_MIN, RETRO_VALUE_MIN, RETRO_VALUE_MIN), cv::Scalar(RETRO_HSV_MAX, RETRO_VALUE_MAX, RETRO_VALUE_MAX), LocalProcessImage);

    // Removes pixles at a certain size, And dilates the image to get rid of gaps
    if (ErosionSize > 0) {
      cv::erode(LocalProcessImage, LocalProcessImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(ErosionSize, ErosionSize)));
      cv::dilate(LocalProcessImage, LocalProcessImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(DialationSize, DialationSize)));
    }
    LocalProcessImage.copyTo(*OutputImage);
  }
}

void CJ::VisionTracking::RetroTrack(cv::Mat *OutputImage, cv::Mat *InputImage, int ErosionSize, int DialationSize) {
  while (Camera.cam.sink.GrabFrame(*InputImage) == 0) {
    std::cout << "Can't Get Input Frame (Retro Track Thread)" << std::endl;
  } 
  std::thread RetroThread(RetroTrackThread, OutputImage, InputImage, ErosionSize, DialationSize);
  RetroThread.detach();
}

void CustomTrackThread(cv::Mat *OutputImage, cv::Mat *InputImage, int HSVColourLowRange, int HSVColourHighRange, int ValueColourLowRange, int ValueColourHighRange, int CamExposure, int ErosionSize, int DialationSize, cs::UsbCamera cam) {
  while (true) {
    cv::cvtColor(*InputImage, LocalProcessImage, cv::COLOR_BGR2HSV); // Uses HSV Spectrum

    // Keeps Only green pixles
    cv::inRange(LocalProcessImage, cv::Scalar(HSVColourLowRange, ValueColourLowRange, ValueColourLowRange), cv::Scalar(HSVColourHighRange, ValueColourHighRange, ValueColourHighRange), LocalProcessImage);
    
    // Removes pixles at a certain size, And dilates the image to get rid of gaps
    if (ErosionSize > 0) {
      cv::erode(LocalProcessImage, LocalProcessImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(ErosionSize, ErosionSize)));
      cv::dilate(LocalProcessImage, LocalProcessImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(DialationSize, DialationSize)));
    }
    LocalProcessImage.copyTo(*OutputImage);
  }
}

void CJ::VisionTracking::CustomTrack(cv::Mat *OutputImage, cv::Mat *InputImage, int HSVColourLowRange, int HSVColourHighRange, int ValueColourLowRange, int ValueColourHighRange, int CamExposure, int ErosionSize, int DialationSize) {
  while (Camera.cam.sink.GrabFrame(*InputImage) == 0) {
    std::cout << "Can't Get Input Frame (Custom Track Thread)" << std::endl;
  }
  std::thread CustomThread(CustomTrackThread, OutputImage, InputImage, HSVColourLowRange, HSVColourHighRange, ValueColourLowRange, ValueColourHighRange, CamExposure, ErosionSize, DialationSize, cam);
  CustomThread.detach();
}
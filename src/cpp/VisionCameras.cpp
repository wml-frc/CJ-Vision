#include "VisTrack.h"


cs::UsbCamera CJ::VisionCamera::Camera::CamSetup(int Port,  double FPS,  int ResHeight,  int ResWidth,  int Exposure,  std::string CamName) {
  cs::UsbCamera cam{CamName, Port};
  cam.SetResolution(ResWidth, ResHeight);
  std::cout << "Reselotion: " << ResWidth << "," << ResHeight << std::endl;
  sink.SetSource(cam);

  cam.SetExposureManual(-100);
  //cam.SetExposureManual(Exposure);
  std::cout << "Exposure: " << Exposure << std::endl;
  cam.SetFPS(FPS);

  std::cout << "Cam Setup Complete" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(5));
  return cam;
}

auto CJ::VisionCamera::Camera::VideoMode(cs::UsbCamera cam, std::string camName) {
  auto video_mode = cam.GetVideoMode();
  output = frc::CameraServer::GetInstance()->PutVideo(camName, video_mode.width, video_mode.height);
  std::cout << "Width: " << video_mode.width << " Height: " << video_mode.height << std::endl;
  return video_mode;
}

cv::Mat CJ::VisionCamera::Camera::ImageReturn(cs::UsbCamera cam, std::string camName) {
  if (CamStartUp) {
    video_modeStartup = CJ::VisionCamera::Camera::VideoMode(cam, camName);
    CamStartUp = false;
  }
  auto video_mode = video_modeStartup;
  cv::Mat ImageSrc{video_mode.height, video_mode.width, CV_8UC3};
  std::cout << "Image Return Complete" << std::endl;
  return ImageSrc;
}
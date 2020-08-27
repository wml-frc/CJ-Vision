#include "Camera.h"

namespace CJ {
  void Camera::camSetup(Cam *cam) {
    if (cam->config.RetroTrack == true) {
      cam->config.Exposure = -100;
    }

    cam->cap.open(cam->config.CamPort);

    while (!cam->cap.isOpened()) {
      std::cout << cam->config.CamName << " Failed to open" << std::endl;
    }
    
    cam->cap.set(cv::CAP_PROP_FRAME_HEIGHT, cam->config.ResHeight);
    cam->cap.set(cv::CAP_PROP_FRAME_WIDTH, cam->config.ResWidth);
  }
}
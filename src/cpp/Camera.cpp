#include "Camera.h"

namespace CJ {
  void Camera::camSetup(Image *image, Cam *cam) {
    if (cam->config.RetroTrack == true) {
      cam->config.Exposure = -100;
    }

    cam->cap.open(cam->config.CamPort, cam->config.apiID);

    if (!cam->cap.isOpened()) {
      std::cout << cam->config.CamName << " Failed to open camera on port: " << cam->config.CamPort << std::endl;
      PROG::set_PROG_RUNNING(false);
    }
    
    cam->cap.set(cv::CAP_PROP_FPS, cam->config.FPS);
    cam->cap.set(cv::CAP_PROP_EXPOSURE, cam->config.Exposure);

    cam->cap.set(cv::CAP_PROP_FRAME_HEIGHT, cam->config.ResHeight);
    cam->cap.set(cv::CAP_PROP_FRAME_WIDTH, cam->config.ResWidth);

    std::thread capture_t(capture, image, cam);
    capture_t.detach();
  }

  void Camera::capture(Image *image, Cam *cam) {
    while (PROG::THREADS_RUNNING()) {
      cam->cap.read(image->image);
      if (image->image.empty()) {
        PROG::set_PROG_RUNNING(false);
      }
    }
  }
}

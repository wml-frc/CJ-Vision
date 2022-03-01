#include "Capture/Camera.h"

namespace CJ {
  Camera::~Camera() {
    CJ_CORE_PRINT_WARN("Destroyed Camera, cap released");
    cap.release();
  }

  int Camera::init() {
    CJ_CORE_PRINT_INFO("Creating OpenCV Cap");

    cap.set(cv::CAP_PROP_FPS, config.fps);
    cap.open(config.port, config.apiID);
    cv::waitKey(3000);
    if (!cap.isOpened()) {
      CJ_CORE_PRINT_ERROR(config.name + " Failed to open on port: " + std::to_string(config.port));
      return 1;
    }

    CJ_CORE_PRINT_INFO("Setting frame res");
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, config.resHeight);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, config.resWidth);


    // Absolute exposure
    if (config.useAbsoluteExposure) {
      CJ_CORE_PRINT_INFO("Auto exposure disabled, (Absolute Exposure)");
      cap.set(cv::CAP_PROP_AUTO_EXPOSURE, config.cap_prop_autoExpose);
      cap.set(cv::CAP_PROP_AUTO_EXPOSURE, config.cap_prop_manualExpose);
      // cap.set(cv::CAP_PROP_EXPOSURE, config.exposure);

      // Script method
      #ifdef CJ_PLATFORM_LINUX
        // char v4l2_script[100] = "v4l2-ctl --set-ctrl=exposure_auto=1 --set-ctrl=exposure_absolute=";
        const std::string v4l2_script = "v4l2-ctl --set-ctrl=exposure_auto=1 --set-ctrl=exposure_absolute=" + std::to_string((int)config.exposure);
        CJ_PRINT_INFO("exposure script: " + v4l2_script);
        const int n = v4l2_script.size();
        char v4l2_script_c[n+1];
        strcpy(v4l2_script_c, v4l2_script.c_str());
        system(v4l2_script_c);
      #endif
      CJ_CORE_PRINT_INFO("Exposure: " + std::to_string(config.exposure));

      // Auto exposure
    } else if (config.autoExposure) {
      CJ_CORE_PRINT_INFO("Auto exposure enabled");
      cap.set(cv::CAP_PROP_AUTO_EXPOSURE, config.cap_prop_manualExpose);
      cap.set(cv::CAP_PROP_AUTO_EXPOSURE, config.cap_prop_autoExpose);

      // Manual Opencv exposure
    } else {
      CJ_CORE_PRINT_INFO("Auto exposure disabled");
      cap.set(cv::CAP_PROP_AUTO_EXPOSURE, config.cap_prop_autoExpose);
      cap.set(cv::CAP_PROP_AUTO_EXPOSURE, config.cap_prop_manualExpose);
      cap.set(cv::CAP_PROP_EXPOSURE, config.exposure);
      CJ_CORE_PRINT_INFO("Exposure: " + std::to_string(config.exposure));
    }

    return 0;
  }

  void Camera::capture(Image &image) {
    cap.read(image.data);
    if (image.data.empty()) {
      CJ_CORE_PRINT_ERROR("Cannot get input image. Image empty");
    }
  }
}
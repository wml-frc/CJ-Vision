#define DEBUG

#ifdef DEBUG
  
  #include "Core.h"

  // #include <opencv2/opencv.hpp>
  // #include <opencv2/objdetect.hpp>
  // #include <opencv2/highgui.hpp>
  // #include <opencv2/imgproc.hpp>
  // #include <opencv2/highgui/highgui.hpp>
  // #include <opencv2/imgproc/imgproc.hpp>
  // #include <opencv2/core/core.hpp>
  // #include <opencv2/core/types.hpp>

  int main(int argc, char** argv) {
    // std::cout << "Program starting" << std::endl;
    // cv::Mat image;
    // cv::VideoCapture cap;

    // int deviceId = 0;
    // int apiID = cv::CAP_ANY;

    // cap.open(deviceId, apiID);

    // if (!cap.isOpened()) {
    //   std::cerr << "ERROR! Unable to open camera\n";
    //   return -1;
    // }

    // std::cout << "Grabbing image" << std::endl;

    // for (;;) {
    //   cap.read(image);

    //   if (image.empty()) {
    //     std::cerr << "ERROR! blank image\n";
    //   }
    // }
    CJ::Core::init();

    CJ::Camera::Cam cam;
    cam.config.CamName = "Brutal Camera";
    cam.config.Exposure = 100;

    CJ::Image image;

    CJ::Core core;

    core.setupVision(&image, &cam);
    
    cv::waitKey(3000);
    while (PROG::PROG_RUNNING()) {
      if (image.image.empty()) {
        std::cout << "Empty image display" << std::endl;
      } else {
        cv::imshow(cam.config.CamName, image.image);
      }
      cv::waitKey(30);
    }
    return 0;
  }

# else 

  int main(int argc, char** argv) {
    printf("Vision Program Starting")
  }
#endif
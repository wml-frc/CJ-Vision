#define DEBUG

#ifdef DEBUG
  #include <opencv2/highgui.hpp>
  #include <opencv2/core.hpp>
  #include <opencv2/videoio.hpp>
  #include <iostream>
  #include <stdio.h>



  int main(int argc, char** argv) {
    std::cout << "Program starting" << std::endl;
    cv::Mat image;
    cv::VideoCapture cap;

    int deviceId = 0;
    int apiID = cv::CAP_ANY;

    cap.open(deviceId, apiID);

    if (!cap.isOpened()) {
      std::cerr << "ERROR! Unable to open camera\n";
      return -1;
    }

    std::cout << "Grabbing image" << std::endl;

    for (;;) {
      cap.read(image);

      if (image.empty()) {
        std::cerr << "ERROR! blank image\n";
      }
    }
    return 0;
  }

# else 

  int main(int argc, char** argv) {
    printf("Vision Program Starting")
  }
#endif
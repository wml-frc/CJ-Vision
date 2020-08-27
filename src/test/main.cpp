#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <stdio.h>
 
int main( int argc, char** argv ) {
  
  cv::Mat image;
  cv::VideoCapture cam;
  cam.open(0);

  while(!cam.isOpened()) {
    std::cout << "Failed to get image" << std::endl;
  }

  for (;;) {
    cam.read(image);
    if (image.empty()) {
      std::cout << "blank image grabbed" << std::endl;
    } else {
      cv::imshow("Image", image);
      if (cvWaitKey(5) >= 0)
        break;
    }
  }
  return 0;
}
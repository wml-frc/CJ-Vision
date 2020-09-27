#include "Output.h"

namespace CJ {
  void Output::Display(Image *image) {
    if (image->data.empty()) {
      std::cerr << "Display Error: Image empty" << std::endl;
    } else {
      // Temp (imshow doesn't work without GUI. << Pi error)
      #ifdef COPROC
        std::cout << "Displaying using socket" << std::endl;
      #else
        cv::imshow(image->name, image->data);
      #endif
    }
    cv::waitKey(30);
  }
}
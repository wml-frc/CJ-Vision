#define DEBUG

#ifdef DEBUG
  
  #include "Core.h"

  int main(int argc, char** argv) {
    // CJ::Core::init();

    // CJ::Camera::Cam cam;
    // cam.config.CamPort = 0;
    // cam.config.CamName = "Debug Cam";
    // cam.config.AutoExposure = true;
    // cam.config.Exposure = 0;
    // cam.config.FPS = 60;

    // CJ::Image inputImage;
    // inputImage.name = "Origin Debug Image";

    // CJ::Image outputImage;
    // outputImage.name = "Output Debug Image";

    // CJ::Core core;

    // std::cout << "Exposure: " << cam.config.Exposure << std::endl;
    // core.setupVision(&inputImage, &cam);

    // CJ::ColourFilter::Options filterOptions;
    // filterOptions.HColourLowRange = 110;
    // filterOptions.HColourHighRange = 255;
    // filterOptions.SColourLowRange = 0;
    // filterOptions.SColourHighRange = 255;
    // filterOptions.VColourLowRange = 0;
    // filterOptions.VColourHighRange = 255;

    // CJ::ColourFilter::filter(&inputImage, &outputImage, filterOptions);

    // CJ::Network cj_nt;
    CJ::Network::test();

    
    // cv::imshow("Input", inputImage.data);
    // cv::imshow("Output", outputImage.data);
    // cv::waitKey(30);

    // while (PROG::PROG_RUNNING()) {
    //   CJ::Output::Display(&inputImage);
    //   CJ::Output::Display(&outputImage);
    // }
    return 0;
  }

# else 

  int main(int argc, char** argv) {
    printf("Vision Program Starting")
  }
#endif
#define DEBUG

#ifdef DEBUG
  
  #include "Core.h"

  int main(int argc, char** argv) {
    CJ::Core::init();

    CJ::Camera::Cam cam;
    cam.config.CamPort = 2;
    cam.config.CamName = "Debug Cam";
    cam.config.AutoExposure = false;
    cam.config.Exposure = 0;
    cam.config.FPS = 60;

    CJ::Image inputImage;
    inputImage.name = "Origin Debug Image";

    CJ::Image outputImage;
    outputImage.name = "Output Debug Image";

    CJ::Core core;

    std::cout << "Exposure: " << cam.config.Exposure << std::endl;
    core.setupVision(&inputImage, &cam);

    CJ::ColourFilter::Options filterOptions;
    filterOptions.HColourLowRange = CJ::ColourFilter::RETRO_H_MIN;
    filterOptions.HColourHighRange = CJ::ColourFilter::RETRO_H_MAX;
    filterOptions.SColourLowRange = CJ::ColourFilter::RETRO_S_MIN;
    filterOptions.SColourHighRange = CJ::ColourFilter::RETRO_S_MAX;
    filterOptions.VColourLowRange = CJ::ColourFilter::RETRO_V_MIN;
    filterOptions.VColourHighRange = CJ::ColourFilter::RETRO_V_MAX;

    // CJ::ColourFilter::filter(&inputImage, &outputImage, filterOptions);


    std::thread server_t(CJ::Network::Send, &inputImage.data);
    std::thread client_t(CJ::Network::Receive, &outputImage.data);

    server_t.join();
    client_t.join();

    
    cv::imshow("Input", inputImage.data);
    cv::imshow("Output", outputImage.data);
    cv::waitKey(30);

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
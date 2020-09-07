#define DEBUG

#ifdef DEBUG
  
  #include "Core.h"

  int main(int argc, char** argv) {
    CJ::Core::init();

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
    std::cout << "main executed" << std::endl;


    CJ::Network::Control nt;
    CJ::Network::Control::server s_nt;
    CJ::Network::Control::client c_nt;

    s_nt.setPort(3000);
    c_nt.setPort(3000);

    s_nt.init();
    c_nt.init();

    CJ::Network::dataPacket dpSend;
    dpSend.id[0] = 'c';
    dpSend.IntegerValues[0] = 4;
    dpSend.BooleanValues[0] = true;
    dpSend.DoubleValues[0] = 5.212;
    dpSend.DoubleValues[511] = 2.132;

    CJ::Network::dataPacket dpGet;

    c_nt.registerSend(&dpSend);
    s_nt.registerReceive(&dpGet);
    

    // cv::imshow("Input", inputImage.data);
    // cv::imshow("Output", outputImage.data);
    // cv::waitKey(30);

    while (PROG::PROG_RUNNING()) {
      system("clear");
      dpSend.DoubleValues[511] += 0.0001;
      std::cout << "dpGet data true: " << dpGet.dataTrue << std::endl;
      std::cout << "dpGet ID: " << dpGet.id[0] << std::endl;
      std::cout << "dpGet Integers: " << dpGet.IntegerValues[0] << std::endl;
      std::cout << "dpGet Booleans: " << dpGet.BooleanValues[0] << std::endl;
      std::cout << "dpGet Doubles: " << dpGet.DoubleValues[0] << std::endl;
      std::cout << "dpGet Double 511: " << dpGet.DoubleValues[511] << std::endl; 
      // CJ::Output::Display(&inputImage);
      // CJ::Output::Display(&outputImage);
    }
    return 0;
  }

# else 

  int main(int argc, char** argv) {
    printf("Vision Program Starting")
  }
#endif
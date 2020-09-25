#define DEBUG

#ifdef DEBUG
  
  #include "Core.h"

  int main(int argc, char** argv) {
    CJ::Core::init();

    CJ::Camera::Cam cam;
    cam.config.ResWidth = 640;
    cam.config.ResHeight = 480;
    cam.config.CamPort = 2;
    cam.config.CamName = "Debug Cam";
    cam.config.AutoExposure = true;
    cam.config.Exposure = 50;
    cam.config.FPS = 60;

    CJ::Image inputImage;
    inputImage.name = "Origin Debug Image";

    CJ::Image outputImage;
    outputImage.name = "Output Debug Image";

    CJ::Core core;

    std::cout << "Exposure: " << cam.config.Exposure << std::endl;
    core.setupVision(&inputImage, &cam);

    CJ::ColourFilter::Options filterOptions;
    filterOptions.HColourLowRange = 20;
    filterOptions.HColourHighRange = 35;
    filterOptions.SColourLowRange = 100;
    filterOptions.SColourHighRange = 255;
    filterOptions.VColourLowRange = 100;
    filterOptions.VColourHighRange = 255;

    CJ::ColourFilter::filter(&inputImage, &outputImage, filterOptions);


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

    while (PROG::PROG_RUNNING()) {
      system("clear");
      dpSend.DoubleValues[511] += 0.0001;
      std::cout << "dpGet data true: " << dpGet.dataTrue << std::endl;
      std::cout << "dpGet ID: " << dpGet.id[0] << std::endl;
      std::cout << "dpGet Integers: " << dpGet.IntegerValues[0] << std::endl;
      std::cout << "dpGet Booleans: " << dpGet.BooleanValues[0] << std::endl;
      std::cout << "dpGet Doubles: " << dpGet.DoubleValues[0] << std::endl;
      std::cout << "dpGet Double 511: " << dpGet.DoubleValues[511] << std::endl; 
      CJ::Output::Display(&inputImage);
      CJ::Output::Display(&outputImage);
    }
    printf("\nProgram Ended\n");
    return 0;
  }

# else 
  #ifdef VISION
    #include "Vision.h"
    int main(int argc, char** argv) {
      Vision v;
      printf("Vision Program Starting")
      CJ::Core::init();
      v.init();
      while (PROG::PROG_RUNNING()) {
        v.periodic();
      }
      printf("Vision Program Exited");
    }
  #endif
#endif
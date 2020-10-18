#ifdef DEBUG
	#define CJ_BUFFSIZE 10
	#include "Core.h"

	int main(int argc, char** argv) {
		CJ::Core::init();

		// CJ::Camera::Cam cam;
		// cam.config.ResWidth = 640;
		// cam.config.ResHeight = 480;
		// cam.config.CamPort = 0;
		// cam.config.CamName = "Debug Cam";
		// cam.config.AutoExposure = true;
		// cam.config.Exposure = 50;
		// cam.config.FPS = 60;

		// CJ::Image inputImage;
		// inputImage.name = "Origin Debug Image";

		// CJ::Image filteredImage;
		// filteredImage.name = "Output Debug Image";

		// CJ::Image cannyOutput;
		// CJ::Image contourOutput;

		// std::cout << "Exposure: " << cam.config.Exposure << std::endl;
		// CJ::Core::setupVision(&inputImage, &cam);

		// CJ::ColourFilter::Options filterOptions;
		// filterOptions.HColourLowRange = 20;
		// filterOptions.HColourHighRange = 35;
		// filterOptions.SColourLowRange = 100;
		// filterOptions.SColourHighRange = 255;
		// filterOptions.VColourLowRange = 100;
		// filterOptions.VColourHighRange = 255;
		// filterOptions.GreyScale = false;

		// CJ::ColourFilter::filter(&inputImage, &filteredImage, filterOptions);

		// CJ::Contours::detectContours(&filteredImage, &contourOutput, 100);

		// Networking 
		#ifdef __linux__
		CJ::Network::Control nt;
		CJ::Network::Control::server s_nt;
		CJ::Network::Control::client c_nt;

		s_nt.setPort(3000);
		// char ip[] = "192.168.178.196";
		// c_nt.setIP(ip);
		c_nt.setPort(3000);

		s_nt.init();
		c_nt.init();

		CJ::Network::dataPacket dpSend;
		dpSend.id[0] = 'c';
		dpSend.IntegerValues[0] = 4;
		dpSend.BooleanValues[0] = true;
		dpSend.DoubleValues[0] = 5.212;
		// dpSend.DoubleValues[0] = 2.132;

		CJ::Network::dataPacket dpGet;

		c_nt.registerSend(&dpSend);
		s_nt.registerReceive(&dpGet);
		#endif

		while (PROG::PROG_RUNNING()) {
			system("clear");
			#ifdef __linux__
			std::cout << "Networking Test" << std::endl;
			std::cout << "BufferSize_Server: " << s_nt.getBuffer() << std::endl;
			std::cout << "BufferSize_Client: " << c_nt.getBuffer() << std::endl;
			std::cout << "Ip Adress: " << c_nt.getIP() << std::endl;
			std::cout << "Port: " << c_nt.getPort() << std::endl;
			dpSend.DoubleValues[0] += 0.0001;
			std::cout << "dpGet data true: " << dpGet.dataTrue << std::endl;
			std::cout << "dpGet ID: " << dpGet.id[0] << std::endl;
			std::cout << "dpGet Integers: " << dpGet.IntegerValues[0] << std::endl;
			std::cout << "dpGet Booleans: " << dpGet.BooleanValues[0] << std::endl;
			std::cout << "dpGet Doubles: " << dpGet.DoubleValues[0] << std::endl;
			std::cout << "dpGet Double 511: " << dpGet.DoubleValues[0] << std::endl; 
			#endif
			// CJ::Output::Display(&inputImage);
			// CJ::Output::Display(&filteredImage);
			// CJ::Output::Display(&contourOutput);
		}
		printf("\nProgram Ended\n");
		return 0;
	}

#else 
	#include "Vision.h"
	#ifdef VISION_H
		#include "Core.h"
		int main(int argc, char** argv) {
			Vision v;
			printf("Vision Program Starting");
			CJ::Core::init();
			v.Init();
			while (PROG::PROG_RUNNING()) {
				v.Periodic();
			}
			printf("Vision Program Exited");
		}
	#else
		#error "#define VISION_H not found"
	#endif
#endif
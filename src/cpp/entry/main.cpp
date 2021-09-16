#ifdef DEBUG_CJ
	// #define CJ_BUFFSIZE 1
	#include "Core.h"

	int main(int argc, char** argv) {
		CJ::Core::init();

		CJ::Camera::Cam cam;
		cam.config.ResWidth = 640;
		cam.config.ResHeight = 480;
		cam.config.CamPort = 0;
		cam.config.CamName = "Debug Cam";
		cam.config.AutoExposure = true;
		cam.config.Exposure = 50;
		cam.config.FPS = 60;

		CJ::Image inputImage;
		inputImage.name = "Origin Debug Image";

		CJ::Image filteredImage;
		filteredImage.name = "Output Debug Image";

		CJ::Image contourOutput;

		std::cout << "Exposure: " << cam.config.Exposure << std::endl;
		CJ::Core::setupVision(&inputImage, &cam);

		CJ::ColourFilter::Options filterOptions;
		filterOptions.HColourLowRange = 20;
		filterOptions.HColourHighRange = 35;
		filterOptions.SColourLowRange = 100;
		filterOptions.SColourHighRange = 255;
		filterOptions.VColourLowRange = 100;
		filterOptions.VColourHighRange = 255;
		filterOptions.GreyScale = false;

		CJ::ColourFilter::filter(&inputImage, &filteredImage, filterOptions);

		CJ::Contours::detectContours(&filteredImage, &contourOutput, 100);

		#ifdef __linux__
		// UDP_TransferNT::Server server;
		// UDP_TransferNT::Client client;

		// client.getSocket().setIP("127.0.0.1");
		// server.init();
		// client.init();

		// UDP_TransferNT::DataPacket dpSend, dpRecv;
		// float value = 1;

		// dpSend.setDecimals(0, value);
		// server.registerSend(dpSend);
		// client.registerRecv(dpRecv);

		while (PROG::PROG_RUNNING()) {
			// server.send(dpSend);
			// client.recv(dpRecv);
			// system("clear");
			// std::cout << "Client receive: " << dpRecv.getDecimals(0) << std::endl;
			// value += 0.001;
			// std::cout << "value real: " << value << std::endl;
			// dpSend.setDecimals(0, value);
		}	
		#endif
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
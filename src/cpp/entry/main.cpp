#define DEBUG
#define CJ_CONNECTION_TIMEOUT 10

#ifdef DEBUG
	// #define CJ_BUFFSIZE 1
	#include "Core.h"


	using ST = CJ::Network::Utilities::StateController::State;

	int main(int argc, char** argv) {
		// CJ::Network::Client client;
		// CJ::Network::Server server;

		// std::cout << "Client IP Before: " << client.getIP() << std::endl;
		// client.setIP("106.69.35.111");
		// std::cout << "Client IP After: " << client.getIP() << std::endl;
		
		// server.init();
		// client.init();

		CJ::Core::init();

		CJ::Camera::Cam cam;
		cam.config.CamPort = 0;

		CJ::Image inputImage;
		inputImage.name = "Input image";

		std::string penImg = CJ_DEPLOY_FOLDER + (std::string)"/pen.jpeg";

		cv::Mat image = cv::imread(penImg, cv::IMREAD_GRAYSCALE);

		CJ::Core::setupVision(&inputImage, &cam);



		while (true) {
			// std::cout << "Connection state: ";
			// std::cout << (int)server.getStateController()->getState() << std::endl;
			CJ::Output::Display(&inputImage);
			cv::imshow("Training image", image);
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
			return 0;
		}
	#else
		#error "#define VISION_H not found"
	#endif
#endif
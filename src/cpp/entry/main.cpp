#define DEBUG
#define CJ_CONNECTION_TIMEOUT 10

#ifdef DEBUG
	// #define CJ_BUFFSIZE 1
	#include "Core.h"


	using ST = CJ::Network::Utilities::StateController::State;

	int main(int argc, char** argv) {
		CJ::Network::Client client;
		CJ::Network::Server server;
		
		server.init();
		client.init();

		while (true) {
			// std::cout << "Connection state: ";
			// std::cout << (int)server.getStateController()->getState() << std::endl;
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
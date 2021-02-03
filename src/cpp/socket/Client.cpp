#include "socket/Network.h"

namespace CJ {
	void Network::Client::_init(Vals_C *vs, StateController *stc) {
		stc->setState(StateController::State::CONNECTING);

		if ((vs->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			std::cout << "Socket Creation error" << std::endl;
			stc->setState(StateController::State::ERROR);
			return;
		}

		std::cout << "Socket created" << std::endl;

		vs->serv_addr.sin_family = AF_INET;
		vs->serv_addr.sin_port = htons(vs->port);

		if (inet_pton(AF_INET, vs->ipAddress, &vs->serv_addr.sin_addr) <= 0) {
			std::cout << "Invalid IP adress" << std::endl;
			stc->setState(StateController::State::ERROR);
			return;
		}

		std::cout << "Address set" << std::endl;

		while(connect(vs->sock, (struct sockaddr *) &vs->serv_addr, sizeof(vs->serv_addr)) < 0) {
			std::cout << "Connection Failing" << std::endl;
			stc->setState(StateController::State::ERROR);
		}

		std::cout << "Connected to server" << std::endl;
		std::cout << "Performing handshake" << std::endl;

		char version[CJ_NETWORK_VERSION_SIZE];
		int size = recv(vs->sock, &version, sizeof(version), 0);
		if (size != -1 || size != 0) {
			if (strcmp(CJ_NETWORK_VERSION, version) == 0) {
				stc->setState(StateController::State::CONNECTED);
				std::cout << "Handshake successful" << std::endl;
			} else {
				stc->setState(StateController::State::ERROR);
				std::cout << "Error during handshake: versions do not match" << std::endl;
				std::cout << "Client version: " << version << std::endl;
				std::cout << "Server version: " << CJ_NETWORK_VERSION << std::endl;
			}
		} else {
			stc->setState(StateController::State::ERROR);
			std::cout << "Connection error during handshake" << std::endl;
		}

		std::cout << "Connection Successful" << std::endl;
		std::cout << "Connected to port: " << vs->port << std::endl;
		std::cout << "Connected to IP: " << vs->ipAddress << std::endl;
	}
}
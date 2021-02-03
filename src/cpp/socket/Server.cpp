#include "socket/Network.h"

namespace CJ {
	void Network::Server::_init(Vals_S *vs, StateController *stc) {
		std::cout << "Server Initialization" << std::endl;
		stc->setState(StateController::State::CONNECTING);

		if ((vs->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
			perror("Socket failed");
			stc->setState(StateController::State::ERROR);
			return;
		}

		std::cout << "Socket created" << std::endl;

		if (setsockopt(vs->server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &vs->opt, sizeof(vs->opt))) {
			std::cout << "setsockopt fail" << std::endl;
			stc->setState(StateController::State::ERROR);
			return;
		}

		std::cout << "set socket successful" << std::endl;

		vs->address.sin_family = AF_INET;
		vs->address.sin_addr.s_addr = INADDR_ANY;
		vs->address.sin_port = htons(vs->port);

		if (bind(vs->server_fd, (struct sockaddr *)&vs->address, sizeof(vs->address))<0) {
			std::cout << "bind failed";
			stc->setState(StateController::State::ERROR);
			return;
		}

		std::cout << "Bind success" << std::endl;

		if (listen(vs->server_fd, 3) < 0) {
			stc->setState(StateController::State::ERROR);
			std::cout << "Listen failed";
		}

		std::cout << "Listen success" << std::endl;

		if ((vs->new_socket = accept(vs->server_fd, (struct sockaddr *)&vs->address, (socklen_t*)&vs->addrlen))<0) {
			std::cout << "accept failed";
			stc->setState(StateController::State::ERROR);
		}

		// std::cout << "Performing handshake with " << vs->n_clients << " clients" << std::endl;
		// Send out data
		send(vs->new_socket, &CJ_NETWORK_VERSION, sizeof(CJ_NETWORK_VERSION), 0);
		// Receive network version
		char version[CJ_NETWORK_VERSION_SIZE];
		int size = recv(vs->new_socket, &version, sizeof(version), 0);
		if (size != -1 || size != 0) {
			if (strcmp(CJ_NETWORK_VERSION, version) == 0) {
				stc->setState(StateController::State::CONNECTED);
				std::cout << "Handshake successful" << std::endl;
				std::cout << "Server Creation Sucessful" << std::endl;
				std::cout << "Server set on port: " << vs->port << std::endl;
			} else {
				stc->setState(StateController::State::ERROR);
				std::cout << "Error during handshake: versions do not match" << std::endl;
				std::cout << "Server version: " << CJ_NETWORK_VERSION << std::endl;
				std::cout << "Client version: " << version << std::endl;
			}
		} else {
			stc->setState(StateController::State::ERROR);
			std::cout << "Connection error during handshake" << std::endl;
		}
	}
}
#include "socket/Network.h"

namespace CJ {
	void Network::Server::_init(Vals_S *vs, StateController *stc) {
		// std::cout << "Server Initialization" << std::endl;
		// stc->setState(StateController::State::CONNECTING);

		// if ((vs->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		// 	perror("Socket failed");
		// 	stc->setState(StateController::State::ERROR);
		// 	return;
		// }

		// std::cout << "Socket created" << std::endl;

		// if (setsockopt(vs->server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &vs->opt, sizeof(vs->opt))) {
		// 	std::cout << "setsockopt fail" << std::endl;
		// 	stc->setState(StateController::State::ERROR);
		// 	return;
		// }

		// std::cout << "set socket successful" << std::endl;

		// vs->address.sin_family = AF_INET;
		// vs->address.sin_addr.s_addr = INADDR_ANY;
		// vs->address.sin_port = htons(vs->port);

		// if (bind(vs->server_fd, (struct sockaddr *)&vs->address, sizeof(vs->address))<0) {
		// 	std::cout << "bind failed";
		// 	stc->setState(StateController::State::ERROR);
		// 	return;
		// }

		// std::cout << "Bind success" << std::endl;

		// if (listen(vs->server_fd, 3) < 0) {
		// 	stc->setState(StateController::State::ERROR);
		// 	std::cout << "Listen failed";
		// }

		// std::cout << "Listen success" << std::endl;

		// if ((vs->new_socket = accept(vs->server_fd, (struct sockaddr *)&vs->address, (socklen_t*)&vs->addrlen))<0) {
		// 	std::cout << "accept failed";
		// 	stc->setState(StateController::State::ERROR);
		// }

		// // std::cout << "Performing handshake with " << vs->n_clients << " clients" << std::endl;
		// // Send out data
		// send(vs->new_socket, &CJ_NETWORK_VERSION, sizeof(CJ_NETWORK_VERSION), 0);
		// // Receive network version
		// char version[CJ_NETWORK_VERSION_SIZE];
		// int size = recv(vs->new_socket, &version, sizeof(version), 0);
		// if (size != -1 || size != 0) {
		// 	if (strcmp(CJ_NETWORK_VERSION, version) == 0) {
		// 		stc->setState(StateController::State::CONNECTED);
		// 		std::cout << "Handshake successful" << std::endl;
		// 		std::cout << "Server Creation Sucessful" << std::endl;
		// 		std::cout << "Server set on port: " << vs->port << std::endl;
		// 	} else {
		// 		stc->setState(StateController::State::ERROR);
		// 		std::cout << "Error during handshake: versions do not match" << std::endl;
		// 		std::cout << "Server version: " << CJ_NETWORK_VERSION << std::endl;
		// 		std::cout << "Client version: " << version << std::endl;
		// 	}
		// } else {
		// 	stc->setState(StateController::State::ERROR);
		// 	std::cout << "Connection error during handshake" << std::endl;
		// }


		std::cout << "Server starting..." << std::endl;
		stc->setState(StateController::State::CONNECTING);

		// Set socket discriptor
		fd_set readfds;

		// Daemon Echo message
		char *StrtEchoMsg = (char *)"ECHO Daemon v";
		char *EndEchoMsg = (char *)"\r\n";
		char *handshakeMsg = new char[strlen(StrtEchoMsg)+strlen(CJ_NETWORK_VERSION)+strlen(EndEchoMsg)+1];
		strcat(handshakeMsg, StrtEchoMsg);
		strcat(handshakeMsg, CJ_NETWORK_VERSION);
		strcat(handshakeMsg, EndEchoMsg);

		// Init all clients
		for (vs->i = 0; vs->i < CJ_N_CLIENTS; vs->i++) {
			vs->client_socket[vs->i] = 0;
		}

		// create master socket
		if ((vs->master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
			perror("Socket failed");
			exit(EXIT_FAILURE);
		}

		// Set master socket to allow multiple connections (works without, but just in case)
		if (setsockopt(vs->master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&vs->opt, sizeof(vs->opt)) > 0) {
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}

		// Type of socket created
		vs->address.sin_family = AF_INET;
		vs->address.sin_addr.s_addr = INADDR_ANY;
		vs->address.sin_port = htons(CJ_DEFAULT_PORT);

		// bind the socket to default port
		if (bind(vs->master_socket, (struct sockaddr *)&vs->address, sizeof(vs->address)) < 0) {
			perror("Bind failed");
			exit(EXIT_FAILURE);
		}
		
		std::cout << "Listener on port: " << (int)CJ_DEFAULT_PORT << std::endl;

		// Specify max number of pending clients
		if (listen(vs->master_socket, CJ_N_PENDING) < 0) {
			perror("Listen failed");
			exit(EXIT_FAILURE);
		}

		stc->setState(StateController::State::CONNECTED);

		vs->addrlen = sizeof(vs->address);
		puts("Waiting for connections...");

		while (true) {

			// clear the socket set (not a mechanical joke)
			FD_ZERO(&readfds);

			// Add master socket to set
			FD_SET(vs->master_socket, &readfds);
			vs->max_sd = vs->master_socket;

			//add child sockets to set  
			for (vs->i = 0; vs->i < CJ_N_CLIENTS; vs->i++) {
				//socket descriptor
				vs->sd = vs->client_socket[vs->i];
				//if valid socket descriptor then add to read list  
				if(vs->sd > 0) {
					FD_SET(vs->sd , &readfds);
				}

				//highest file descriptor number, need it for the select function
				if(vs->sd > vs->max_sd) {
					vs->max_sd = vs->sd;
				}
			}

			// Wait for activity on one of the sockets
			vs->activity = select(vs->max_sd + 1, &readfds, NULL, NULL, NULL);

			if ((vs->activity < 0) && (errno != EINTR)) {
				std::cout << "Select error" << std::endl;
			}

			// if something happened on the master socket (incoming connection)
			if (FD_ISSET(vs->master_socket, &readfds)) {
				if ((vs->new_socket = accept(vs->master_socket, (struct sockaddr *)&vs->address, (socklen_t *)&vs->addrlen)) < 0) {
					perror("Accept error");
					exit(EXIT_FAILURE);
				}

				std::cout << "New connection, socket fd is " << vs->new_socket << ", ip is: " << inet_ntoa(vs->address.sin_addr) << ", port: " << ntohs(vs->address.sin_port) << std::endl;

				// Send new connection handshake message
				if ((int)send(vs->new_socket, handshakeMsg, strlen(handshakeMsg), 0) != (int)strlen(handshakeMsg)) {
					perror("Send error");
				}

				puts("Handshake message sent");

				// add new socket to array of sockets
				for (vs->i = 0; vs->i < CJ_N_CLIENTS; vs->i++) {
					if (vs->client_socket[vs->i] == 0) {
						vs->client_socket[vs->i] = vs->new_socket;
						std::cout << "Adding to list of sockets as" << vs->i;
						break;
					}
				}
			}

			for (vs->i = 0; vs->i < CJ_N_CLIENTS; vs->i++) {
				vs->sd = vs->client_socket[vs->i];

				if (FD_ISSET(vs->sd, &readfds)) {
					// Check if it was for closing, and read incoming message
					if ((vs->valread = read(vs->sd, vs->buffer, CJ_BUFFER_SIZE)) == 0) {
						// some client is disconnected, get details and print
						getpeername(vs->sd, (struct sockaddr*)&vs->address, (socklen_t*)&vs->addrlen);
						std::cout << "Host disconnected, ";
						std::cout << "ip " << inet_ntoa(vs->address.sin_addr);
						std::cout << ", port " << ntohs(vs->address.sin_port) << std::endl;

						close(vs->sd);
						vs->client_socket[vs->i] = 0;
					} else {
						// Echo back message

						vs->buffer[vs->valread] = '\0';
						send(vs->sd, vs->buffer, strlen(vs->buffer), 0);
					}
				}
			}
		}
	}
}
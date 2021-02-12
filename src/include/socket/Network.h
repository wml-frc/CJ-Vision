#ifndef CJ_NETWORK
#define CJ_NETWORK

#ifdef __linux__

/**
 * Linux Socket libs
 */
#include <unistd.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>

/**
 * Std libs
 */
#include <stdlib.h> 
#include <errno.h>
#include <vector>
#include <string.h> 
#include <thread>
#include <iostream>

/**
 * Precompiled control values
 */
#define CJ_DEFAULT_PORT 5800 // 5800:5810 allowed ports in FRC

/**
 * Data size, controls how many values each datapacket array can send. (Lower the faster)
 */
#ifndef CJ_DATA_SIZE
#define CJ_DATA_SIZE 4
#endif

/**
 * Buffersize
 */
#ifndef CJ_BUFFER_SIZE
#define CJ_BUFFER_SIZE 1025 // 1k
#endif

/**
 * Ip length, changes how long the ip adress is, default is 20 characters
 */
#ifndef CJ_IP_LENGTH
#define CJ_IP_LENGTH 20
#endif

/**
 * Define max number of clients that can connect to server
 */
#ifndef CJ_N_CLIENTS
#define CJ_N_CLIENTS 10
#endif

/**
 * Define max number of pending clients
 */
#ifndef CJ_N_PENDING
#define CJ_N_PENDING 3 // Allows 3 clients to be pending
#endif


/**
 * Define timeout while connecting (how many times the program should try to connect)
 * set -1 for infinite
 */
#ifndef CJ_CONNECTION_TIMEOUT
#define CJ_CONNECTION_TIMEOUT 10 // Tries 10 times before giving up
#endif

#define CJ_NETWORK_VERSION "1.5" // Used during handshake to confirm, connection fails if either client or server version is not synced
#define CJ_NETWORK_VERSION_SIZE 5

namespace CJ {
	class Network {
	 public:

		struct Utilities {
			/**
			 * Data packet
			 */
			struct DataPacket {
				// Data check
				bool dataTrue = false;
				int dataSize = CJ_DATA_SIZE;

				// User data
				char characterValue[CJ_DATA_SIZE] {0};
				int integerValue[CJ_DATA_SIZE] {0};
				bool booleanValue[CJ_DATA_SIZE] {0};
				double doubleValue[CJ_DATA_SIZE] {0};
			};


			/**
			 * Server connection values
			 */
			struct Vals_S {

				// Old values
				// uint16_t port = CJ_DEFAULT_PORT;
				// int server_fd = 0, new_socket = 0, valread = 0, n_clients = 1;
				// int opt = 0;
				// struct sockaddr_in address;
				// int addrlen = sizeof(address);

				// Multi client
				int opt = true;
				int master_socket, addrlen, new_socket, client_socket[CJ_N_CLIENTS],
				activity, i, valread, sd;
				int max_sd;
				struct sockaddr_in address;
				char buffer[CJ_BUFFER_SIZE];
			};

			/**
			 * Client values
			 */
			struct Vals_C {
				uint16_t port = CJ_DEFAULT_PORT;
				char ipAddress[CJ_IP_LENGTH] = "127.0.0.1";
				int sock = 0, valread = 0;
				struct sockaddr_in serv_addr;
			};

			/**
			 * State Controller
			 */
			struct StateController {
				/**
				 * Collection of network states
				 */
				enum class State {
					IDLE = 0,
					CONNECTING,
					CONNECTED,
					ERROR
				}; State _st{ State::IDLE };

				/**
				 * Set state
				 */
				void setState(State st) { _st = st; }

				/**
				 * Get state
				 */
				State getState() { return _st; }
			};
		};

	 private:
		class Serialization {

			/**
			 * Serialize datapacket into bytes
			 */
			static void serialize();

			/**
			 * deserialize bytes into datapacket
			 */
			static void deserialize();
		};

	 public:

		/**
		 * Server Logical class
		 */
		class Server : public Utilities, public Serialization {
		 private:
			Vals_S vs_l;
			StateController stc_l;

			static void _init(Vals_S *vs, StateController *stc);
			static void _init_thread(Vals_S *vs, StateController *stc) {
				int timeout = 1;
				while (stc->getState() != StateController::State::CONNECTED) {
					std::cout << "Try count: " << timeout << std::endl;
					if (CJ_CONNECTION_TIMEOUT != -1) {
						if (timeout > CJ_CONNECTION_TIMEOUT) {
							break;
						}
					}

					try {
						_init(vs, stc);
					} catch(const std::exception& e) {
						std::cerr << e.what() << '\n';
					}
					timeout++;
				}
			}
		 public:

			/**
			 * Sets up server
			 */
			void init(Vals_S vs) {
				this->vs_l = vs;
				_init_thread(&this->vs_l, &this->stc_l);
				// std::thread init_t(_init_thread, &this->vs_l, &this->stc_l);
				// init_t.detach();
			}

			/**
			 * Sets up server
			 */
			void init() {
				init(this->vs_l);
			}

			/**
			 * Get state controller
			 */
			StateController *getStateController() { return &this->stc_l; }
		};

		/**
		 * Client Logical class
		 */
		class Client : public Utilities, public Serialization {
		 private:
			Vals_C vs_l;
			StateController stc_l;

			static void _init(Vals_C *vs, StateController *stc);
			static void _init_thread(Vals_C *vs, StateController *stc) {
				int timeout = 1;
				while (stc->getState() != StateController::State::CONNECTED) {
					std::cout << "Try count: " << timeout << std::endl;
					if (CJ_CONNECTION_TIMEOUT != -1) {
						if (timeout > CJ_CONNECTION_TIMEOUT) {
							break;
						}
					}

					try {
						_init(vs, stc);
					} catch(const std::exception& e) {
						std::cerr << e.what() << '\n';
					}
					timeout++;
				}
			}
		 public:

			/**
			 * Setup Client
			 */
			void init(Vals_C vs) {
				this->vs_l = vs;
				std::thread init_t(_init_thread, &this->vs_l, &this->stc_l);
				init_t.detach();
			}

			/**
			 * Sets up Client
			 */
			void init() {
				init(this->vs_l);
			}

			/**
			 * Get state controller
			 */
			StateController *getStateController() { return &this->stc_l; }

			/**
			 * Set IP Address
			 */
			void setIP(const char *ip) { strncpy(this->vs_l.ipAddress, ip, CJ_IP_LENGTH); }

			/**
			 * Get IP Address
			 */
			const char *getIP() { return this->vs_l.ipAddress; }
		};
	};
};

#endif

#endif
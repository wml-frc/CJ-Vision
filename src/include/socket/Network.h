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

/**
 * Std libs
 */
#include <stdlib.h> 
#include <vector>
#include <string.h> 
#include <thread>
#include <iostream>

/**
 * Precompiled control values
 */
#define CJ_DEFAULT_PORT 5800

/**
 * Data size, controls how many values each datapacket array can send. (Lower the faster)
 */
#ifndef CJ_DATA_SIZE
#define CJ_DATA_SIZE 4
#endif

/**
 * Ip length, changes how long the ip adress is, default is 20 characters
 */
#ifndef CJ_IP_LENGTH
#define CJ_IP_LENGTH 20
#endif



#define CJ_NETWORK_VERSION "1.4" // Used during handshake to confirm, connection fails if either client or server version is not synced
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
				uint16_t port = CJ_DEFAULT_PORT;
				int server_fd = 0, new_socket = 0, valread = 0, n_clients = 1;
				int opt = 0;
				struct sockaddr_in address;
				int addrlen = sizeof(address);
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
		 public:

			/**
			 * Sets up server
			 */
			void init(Vals_S vs) {
				this->vs_l = vs;
				_init(&this->vs_l, &this->stc_l);
			}

			/**
			 * Sets up server
			 */
			void init() {
				_init(&this->vs_l, &this->stc_l);
			}
		};

		/**
		 * Client Logical class
		 */
		class Client : public Utilities, public Serialization {
		 private:
			Vals_C vs_l;
			StateController stc_l;

			static void _init(Vals_C *vs, StateController *stc);
		 public:

			/**
			 * Setup Client
			 */
			void init(Vals_C vs) {
				this->vs_l = vs;
				_init(&this->vs_l, &this->stc_l);
			}

			/**
			 * Sets up Client
			 */
			void init() {
				_init(&this->vs_l, &this->stc_l);
			}
		};
	};
};

#endif

#endif
#ifndef CJ_NETWORK
#define CJ_NETWORK

#ifndef _WIN32

#include <unistd.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <thread>
#include <stdlib.h> 
#include <vector>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#include <iostream>

#define CJ_DEFAULT_PORT 5800 // FRC allows ports 5800:5810
#define CJ_BUFFSIZE 512 // 512 of each datatype

// #define SEND_IMAGE
#ifdef CJ_SEND_IMAGE
	#include <opencv2/opencv.hpp>
	#include <opencv2/objdetect.hpp>
	#include <opencv2/highgui.hpp>
	#include <opencv2/imgproc.hpp>
	#include <opencv2/highgui/highgui.hpp>
	#include <opencv2/imgproc/imgproc.hpp>
	#include <opencv2/core/core.hpp>
	#include <opencv2/core/types.hpp>
	#ifndef IMAGE_CONTAINER_H
	#define IMAGE_CONTAINER_H
		namespace CJ {
			struct Image {
				cv::Mat data;
				std::string name = " ";
				std::vector<std::vector<cv::Point>>contours;
			};
		}
	#endif
#endif


namespace CJ {
	class Network {
	public:
		/**
		 * Data Packet used to send over data
		 */
		struct dataPacket {
			bool dataTrue = false;
			char id[CJ_BUFFSIZE]{0};
			int IntegerValues[CJ_BUFFSIZE]{0};
			bool BooleanValues[CJ_BUFFSIZE]{0};
			double DoubleValues[CJ_BUFFSIZE]{0};
			#ifdef SEND_IMAGE
			Image ImageValues[CJ_BUFFSIZE]{0};
			#endif
		};

		#define PACKETSIZE sizeof(dataPacket)

	private:
		struct statesController {

			enum state {
				IDLE = 0,
				CONNECTING,
				CONNECTED,
				ERROR,
				STOP
			}; state _st{state::IDLE};

			void setState(state st) {_st = st;}
			state getState() {return _st;}
		};

		struct vals_c {
			uint16_t port = CJ_DEFAULT_PORT;
			char ipaddress[20] = "127.0.0.1";
			int sock = 0;
			int valread;
			struct sockaddr_in serv_addr;
		};

		struct vals_s {
			uint16_t port = CJ_DEFAULT_PORT;
			int server_fd, new_socket, valread;
			int opt = 1;
			struct sockaddr_in address;
			int addrlen = sizeof(address);
		};


		/**
		 * Serializaiton
		 */
		class Serialization {
		protected:
			static void serialize(dataPacket *msgPacket, char *data) {
				// Data True
				bool *dtTrue = (bool*)data; 
				*dtTrue = msgPacket->dataTrue; dtTrue++;

				int i = 0;

				// ID's
				char *id = (char*)dtTrue;
				i = 0;
				while (i < CJ_BUFFSIZE) {
					*id = msgPacket->id[i];
					id++;
					i++;
				}

				// Integer Values
				int *intVals = (int*)id;
				i = 0;
				while (i < CJ_BUFFSIZE) {
					*intVals = msgPacket->IntegerValues[i];
					intVals++;
					i++;
				}

				// Boolean Values
				bool *boolVals = (bool*)intVals;
				i = 0;
				while (i < CJ_BUFFSIZE) {
					*boolVals = msgPacket->BooleanValues[i];
					boolVals++;
					i++;
				}

				// Double Values
				double *dblVals = (double*)boolVals;
				i = 0;
				while (i < CJ_BUFFSIZE) {
					*dblVals = msgPacket->DoubleValues[i];
					dblVals++;
					i++;
				}

				// Images
				#ifdef SEND_IMAGE
				Image *imgs = (Image*)dblVals;
				i = 0;
				while (i < CJ_BUFFSIZE) {
					*imgs = msgPacket->ImageValues[i];
					imgs++;
					i++;
				}
				#endif
			}

			static void deserialize(dataPacket *msgPacket, char *data) {
				// Data True
				bool *dtTrue = (bool*)data;
				msgPacket->dataTrue = *dtTrue; dtTrue++;

				int i = 0;

				// ID's
				char *id = (char*)dtTrue;
				i = 0;
				while (i < CJ_BUFFSIZE) {
					msgPacket->id[i] = *id;
					id++;
					i++;
				}

				// Integer Values
				int *intVals = (int*)id;
				i = 0;
				while (i < CJ_BUFFSIZE) {
					msgPacket->IntegerValues[i] = *intVals;
					intVals++;
					i++;
				}

				// Boolean Values
				bool *boolVals = (bool*)intVals;
				i = 0;
				while (i < CJ_BUFFSIZE) {
					msgPacket->BooleanValues[i] = *boolVals;
					boolVals++;
					i++;
				}

				// Double Values
				double *dblVals = (double*)boolVals;
				i = 0;
				while (i < CJ_BUFFSIZE) {
					msgPacket->DoubleValues[i] = *dblVals;
					dblVals++;
					i++;
				}

				// Images
				#ifdef SEND_IMAGE
				Image *imgs = (Image*)dblVals;
				i = 0;
				while (i < CJ_BUFFSIZE) {
					msgPacket->ImageValues[i] = *imgs;
					imgs++;
					i++;
				}
				#endif
			}
		};


		/**
		 * Server Controller
		 */
		class Server : public Serialization {
		protected:
			static void _init(vals_s *vs, statesController *stc) {
				std::cout << "Port: " << vs->port << std::endl;
				stc->setState(statesController::state::CONNECTING);
				std::cout << "Server Init Start" << std::endl;

				// uint16_t port = CJ_DEFAULT_PORT;
				// int server_fd, new_socket, valread;
				// int opt = 1;
				// struct sockaddr_in address;
				// int addrlen = sizeof(address);

				if ((vs->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
					perror("Socket failed");
					stc->setState(statesController::state::ERROR);
					return;
				}

				std::cout << "Socket created" << std::endl;

				if (setsockopt(vs->server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &vs->opt, sizeof(vs->opt))) {
					std::cout << "setsockopt fail" << std::endl;
					stc->setState(statesController::state::ERROR);
					return;
				}

				std::cout << "set socket successful" << std::endl;

				vs->address.sin_family = AF_INET;
				vs->address.sin_addr.s_addr = INADDR_ANY;
				vs->address.sin_port = htons(vs->port);

				if (bind(vs->server_fd, (struct sockaddr *)&vs->address, sizeof(vs->address))<0) {
					std::cout << "bind failed";
					stc->setState(statesController::state::ERROR);
					return;
				}

				std::cout << "Bind success" << std::endl;

				if (listen(vs->server_fd, 3) < 0) {
					stc->setState(statesController::state::ERROR);
					std::cout << "listen failed";
				}

				std::cout << "Listen success" << std::endl;

				if ((vs->new_socket = accept(vs->server_fd, (struct sockaddr *)&vs->address, (socklen_t*)&vs->addrlen))<0) {
					std::cout << "accept failed";
					stc->setState(statesController::state::ERROR);
				}

				std::cout << "Server Creation Sucessful" << std::endl;
				std::cout << "Server set on port: " << vs->port << std::endl;
				stc->setState(statesController::state::CONNECTED);
			}

			static void _send(vals_s *vs, statesController *stc, dataPacket *dataPack) {
				dataPack->dataTrue = true;
				char data[PACKETSIZE];
				if (stc->getState() == statesController::state::CONNECTED) {
					serialize(dataPack, data);
					send(vs->new_socket, &data, sizeof(data), 0);
				}
			}

			static void _registerSend(vals_s *vs, statesController *stc, dataPacket *dataPack) {
				char data[PACKETSIZE];
				while (stc->getState() != statesController::state::STOP) {
					if (stc->getState() == statesController::state::CONNECTED) {
						dataPack->dataTrue = true;
						serialize(dataPack, data);
						send(vs->new_socket, &data, sizeof(data), 0);
					}
				}
			}

			static void _receive(vals_s *vs, statesController *stc, dataPacket *dataPack) {
				dataPacket dp;
				char data[PACKETSIZE];
				if (stc->getState() == statesController::state::CONNECTED) {
					int size = recv(vs->new_socket, &data, sizeof(data), 0);
					if (size != -1 || size != 0) {
						deserialize(&dp, data);
						if (dp.dataTrue) {
							*dataPack = dp;
						}
					}
				}
			}

			static void _registerReceive(vals_s *vs, statesController *stc, dataPacket *dataPack) {
				dataPacket dp;
				while (stc->getState() != statesController::state::STOP) {
					if (stc->getState() == statesController::state::CONNECTED) {
						char data[PACKETSIZE];
						int size;
						size = recv(vs->new_socket, &data, sizeof(data), 0);
						if (size != -1 || size != 0) {
							deserialize(&dp, data);
							if (dp.dataTrue) {
								*dataPack = dp;
							}
						} else {
							std::cout << "No Data" << std::endl;
							stc->setState(statesController::state::ERROR);
						}
					}
				}
			}
		};


		class Client : public Serialization {
		protected:
			static void _init(vals_c *vs, statesController *stc) {
				stc->setState(statesController::state::CONNECTING);

				// uint16_t port = CJ_DEFAULT_PORT;
				// const char *ipaddress = "127.0.0.1";
				// int sock = 0;
				// int valread;
				// struct sockaddr_in serv_addr;

				if ((vs->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
					std::cout << "Socket Creation error" << std::endl;
					stc->setState(statesController::state::ERROR);
					return;
				}

				std::cout << "Socket created" << std::endl;

				vs->serv_addr.sin_family = AF_INET;
				vs->serv_addr.sin_port = htons(vs->port);

				if (inet_pton(AF_INET, vs->ipaddress, &vs->serv_addr.sin_addr) <= 0) {
					std::cout << "Invalid IP adress" << std::endl;
					stc->setState(statesController::state::ERROR);
					return;
				}

				std::cout << "Address set" << std::endl;

				while(connect(vs->sock, (struct sockaddr *) &vs->serv_addr, sizeof(vs->serv_addr)) < 0) {
					std::cout << "Connection Failing" << std::endl;
					stc->setState(statesController::state::ERROR);
				}

				std::cout << "Connection Successful" << std::endl;
				std::cout << "Connected to port: " << vs->port << std::endl;
				std::cout << "Connected to IP: " << vs->ipaddress << std::endl;
				stc->setState(statesController::state::CONNECTED);
			}

			static void _send(vals_c *vs, statesController *stc, dataPacket *dataPack) {
					dataPack->dataTrue = true;
					char data[PACKETSIZE];
				if (stc->getState() == statesController::state::CONNECTED) {
					serialize(dataPack, data);
					send(vs->sock, &data, sizeof(data), 0);
				} 
			}

			static void _registerSend(vals_c *vs, statesController *stc, dataPacket *dataPack) {
				dataPack->dataTrue = true;
				char data[PACKETSIZE];
				while(stc->getState() != statesController::state::STOP) {
					if (stc->getState() == statesController::state::CONNECTED) {
						serialize(dataPack, data);
						send(vs->sock, &data, sizeof(data), 0);
					}
				}
			}

			static void _receive(vals_c *vs, statesController *stc, dataPacket *dataPack) {
				dataPacket dp;
				int size;
				char data[PACKETSIZE];
				if (stc->getState() == statesController::state::CONNECTED) {
					size = recv(vs->sock, &data, sizeof(data), 0);
					if (size != -1 || size != 0) {
						deserialize(&dp, data);
						if (dp.dataTrue) {
							*dataPack = dp;
						}
					}
				}
			}

			static void _registerReceive(vals_c *vs, statesController *stc, dataPacket *dataPack) {
				dataPacket dp;
				while (stc->getState() != statesController::state::STOP) {
					if (stc->getState() == statesController::state::CONNECTED) {
						char data[PACKETSIZE];
						int size;
						size = recv(vs->sock, &data, sizeof(data), 0);
						if (size != -1 || size != 0) {
							deserialize(&dp, data);
							if (dp.dataTrue) {
								*dataPack = dp;
							}
						} else {
							std::cout << "No Data" << std::endl;
							stc->setState(statesController::state::ERROR);
						}
					}
				}
			}
		};

	public:
		class Control {
		protected:
			statesController network_stc{statesController::state::IDLE};
		public:
			class server : public Server {
			private:
				vals_s vs;
				statesController server_stc{statesController::state::IDLE};
			public:
				statesController::state getState() {return server_stc.getState();}
				void init() {
					std::thread sv_init_t(_init, &vs, &server_stc);
					sv_init_t.detach();
				}

				void receive(dataPacket *dataPack) {
					_receive(&vs, &server_stc, dataPack);
				}

				void registerReceive(dataPacket *dataPack) {
					std::thread registerReceive_t(_registerReceive, &vs, &server_stc, dataPack);
					registerReceive_t.detach();
				}

				void send(dataPacket *dataPack) {
					_send(&vs, &server_stc, dataPack);
				}

				void registerSend(dataPacket *dataPack) {
					std::thread registerSend_t(_registerSend, &vs, &server_stc, dataPack);
					registerSend_t.detach();
				}

				void setPort(uint16_t port) {
					vs.port = port;
				}

				uint16_t getPort() {
					return vs.port;
				}
			};

			class client : public Client {
			protected:
				vals_c vs;
				statesController client_stc{statesController::state::IDLE};
			public:
				statesController::state getState() {return client_stc.getState();}
				void init() {
					std::thread cl_init_t(_init, &vs, &client_stc);
					cl_init_t.detach();
				}

				void receive(dataPacket *dataPack) {
					_receive(&vs, &client_stc, dataPack);
				}

				void registerReceive(dataPacket *dataPack) {
					std::thread registerReceive_t(_registerReceive, &vs, &client_stc, dataPack);
					registerReceive_t.detach();
				}

				void send(dataPacket *dataPack) {
					_send(&vs, &client_stc, dataPack);
				}

				void registerSend(dataPacket *dataPack) {
					std::thread registerSend_t(_registerSend, &vs, &client_stc, dataPack);
					registerSend_t.detach();
				}

				void setPort(uint16_t port) {
					vs.port = port;
				}
				void setIP(char ip[20]) {
					strncpy(vs.ipaddress, ip, 20);
					// vs.ipaddress[20] = ip[20];
				}

				uint16_t getPort() {
					return vs.port;
				}

				char* getIP() {
					return vs.ipaddress;
				}
			};
		};
	};
}

#endif // Windows
#endif // !CJ_NETWORK
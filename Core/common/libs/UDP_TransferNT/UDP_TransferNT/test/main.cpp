#include <iostream>
#include <fstream>
#include "UDP_TransferNT.h"

#ifdef NT_UDP_PLATFORM_WINDOWS
#include <Windows.h>
#elif defined(NT_UDP_PLATFORM_UNIX)
#include <time.h>
#endif

#include <thread>

using namespace UDP_TransferNT;

int cycleCount = 20;

void server() {
	Network s_network(Network::Type::SERVER, Network::ConnectionType::IP_SPECIFIC);
	s_network.getSocket().setRecvTimeout(10);
	s_network.init();

	DataPacket dpRecv;
	while (true) {
	// for (int i = 0; i < cycleCount; i++) {

		// char *buffer;
		// memset(buffer, '\0', DEFAULT_BUFFER_SIZE);
		

		// free(buffer);
		dpRecv = s_network.dpRecv(dpRecv);
		std::cout << "DP Recv: " << dpRecv.getCharacters(0) << std::endl;
		std::cout << "DP Recv: " << dpRecv.getIntegers(3) << std::endl;
		std::cout << "DP Recv: " << dpRecv.getBooleans(5) << std::endl;
		std::cout << "DP Recv: " << dpRecv.getDecimals(2) << std::endl;

		#ifdef NT_UDP_PLATFORM_WINDOWS
		// Sleep(1000);
		#elif defined(NT_UDP_PLATFORM_UNIX)
		// sleep(1);
		#endif
		// dpRecv = s_network.dpRecv(dpRecv);
	}
}

void client() {
	Network c_network(Network::Type::CLIENT, Network::ConnectionType::IP_SPECIFIC);
	c_network.getSocket().setIP("192.168.178.210");
	c_network.init();

	DataPacket dpSend;
	float changingVal = 0.4;

	while (true) {
	// for (int i = 0; i < cycleCount; i++) {
		// dpSend.setDecimals(0, 0.5);
		dpSend.setCharacters(0, 'c');
		dpSend.setIntegers(3, 1);
		dpSend.setBooleans(5, true);
		dpSend.setDecimals(2, changingVal);

		changingVal++;


		c_network.dpSend(dpSend);
		// std::cout << "Hello?" << std::endl;
		// const char buffer[DEFAULT_BUFFER_SIZE] = "Hello from client";
		// buffer = (char *)malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
		// buffer = "Hello from client";
		// c_network.raw_send(buffer);
		// free(buffer);
		// std::cout << "Whats the issue here" << std::endl;
		#ifdef NT_UDP_PLATFORM_WINDOWS
		Sleep(1000);
		#elif defined(NT_UDP_PLATFORM_UNIX)
		usleep(1000);
		#endif
	}
}

void divertCout(bool divert = true) {
	std::ofstream out("out.txt");
	std::streambuf *coutbuff = std::cout.rdbuf();
	std::cout.rdbuf(out.rdbuf());
}

int main() {
	std::cout << "Test Run..." << std::endl;
	// divertCout();

	// int test; 

	// std::cout << sizeof(typeid(test)) << " " << sizeof(int) << std::endl;
	// server();
	client();

	// std::thread server_t(server);
	// std::thread client_t(client);

	// server_t.join();
	// client_t.join();

	system("pause");

	return 0;
}
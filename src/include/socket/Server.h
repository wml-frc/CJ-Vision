#include "NetHeader.h"

// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

namespace CJ {
  class Server {
    public:

    static void serializeServer(testStruct *msgPacket, char *data)  {
      int *q = (int*)data; 
      *q = msgPacket->testInt; q++;

      char *p = (char*)q;
      int i = 0;

      while(i < BUFFSIZE) {
        *p = msgPacket->id[i];
        p++;
        i++;
      }
    }

    static void deserializeServer(char *data, testStruct *msgPacket) {
      int *q = (int*)data;
      msgPacket->testInt = *q; q++;
      char *p = (char*)q;
      int i = 0;
      while (i < BUFFSIZE) {
        msgPacket->id[i] = *p;
        p++;
        i++;
      }
    }

    static void server() {
      int server_fd, new_socket, valread;
      struct sockaddr_in address;
      int opt = 1;
      int addrlen = sizeof(address);
      char buffer[1024] = {0};
      const char *hello = "Hello from server";

      // Creating socket file descriptor
      if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        return;
      }

      std::cout << "Progress: ";

      if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cout << "setsockopt fail" << std::endl;
        return;
      }

      address.sin_family = AF_INET;
      address.sin_addr.s_addr = INADDR_ANY;
      address.sin_port = htons(DEFAULT_PORT);

      std::cout << "|===";

      if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        std::cout << "bind failed";
        return;
      }

      std::cout << "|===";

      if (listen(server_fd, 3) < 0) {
        std::cout << "listen failed";
      }

      std::cout << "|===";

      if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        std::cout << "accept failed";
      }

      std::cout << "|===";

      testStruct *test_Struct = new testStruct;
      int size;
      std::string item_id;
      int iten_value;

      char data[PACKETSIZE];
      size = recv(new_socket, &data, sizeof(data), 0);
      deserializeServer(data, test_Struct);

      std::cout << "| ===" << std::endl;
      std::cout << "Complete" << std::endl;
      std::cout << "Received ID: " << test_Struct->id << std::endl;
      std::cout << "Received Data: " << test_Struct->testInt << std::endl;

    }

  };
}
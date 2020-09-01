#include "NetHeader.h"

// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

namespace CJ {
  class Client {
    public:
    static void serializeClient(testStruct *msgPacket, char *data) {
      int *q = (int*)data;	
      *q = msgPacket->testInt;	q++;	
      
      char *p = (char*)q;
      int i = 0;
      while (i < BUFFSIZE) {
        *p = msgPacket->id[i];
        p++;
        i++;
      }
    }

    static void deserializeClient(char *data, testStruct *msgPacket) {
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

    static void client() {
      const char *ipaddress = "127.0.0.1";

      int sock = 0;
      int valread;
      char buffer[1024] = {0};
      struct sockaddr_in serv_addr;

      if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket Creation error" << std::endl;
        return;
      }

      serv_addr.sin_family = AF_INET;
      serv_addr.sin_port = htons(DEFAULT_PORT);

      if (inet_pton(AF_INET, ipaddress, &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid IP adress" << std::endl;
        return;
      }

      while(connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        // std::cout << "Connection Failed" << std::endl;
        // return;
      }

      testStruct *test_Struct = new testStruct;
      std::string ID = "testID";
      strcpy(test_Struct->id, ID.c_str());

      test_Struct->testInt = 2;

      char data[PACKETSIZE];
      serializeClient(test_Struct, data);
      send(sock, &data, sizeof(data), 0);

      std::cout << "Client data sent" << std::endl;
    }
  };
}
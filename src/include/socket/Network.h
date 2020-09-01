#ifndef CJ_NETWORK
#define CJ_NETWORK


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

#define DEFAULT_PORT 13200
#define BUFFSIZE 512

// #define SEND_IMAGE
#ifdef SEND_IMAGE
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

  /**
   * Data Packet used to send over data
   */
  struct dataPacket {
    char id[BUFFSIZE];
    int IntegerValues[BUFFSIZE];
    bool BooleanValues[BUFFSIZE];
    double DoubleValues[BUFFSIZE];
    std::string StringValues[BUFFSIZE];
    #ifdef SEND_IMAGE
    std::vector<Image>ImageValues;
    #endif
  };

  #define PACKETSIZE sizeof(dataPacket)

  /**
   * Serializaiton
   */
  class Serialization {
   public:
    static void serialize(dataPacket *msgPacket, char *data) {
      int *q = (int*)data;
      *q = msgPacket->DoubleValues[0]; q++;
      char *p = (char*)q;
      int i = 0;
      while (i < BUFFSIZE) {
        *p = msgPacket->id[i];
        p++;
        i++;
      }
    }

    static void deserialize(dataPacket *msgPacket, char *data) {
      int *q = (int*)data;
      msgPacket->DoubleValues[0] = *q; q++;
      char *p = (char*)q;
      int i = 0;
      while (i < BUFFSIZE) {
        msgPacket->id[i] = *p;
        p++;
        i++;
      }
    }
  };


  /**
   * Server Controller
   */
  class Server : public Serialization {
   public:

    static void server() {
      int server_fd, new_socket, valread;
      struct sockaddr_in address;
      int opt = 1;
      int addrlen = sizeof(address);
      char buffer[1024] = {0};

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

      dataPacket *data_Packet = new dataPacket;
      int size;
      std::string item_id;
      int iten_value;

      char data[PACKETSIZE];
      size = recv(new_socket, &data, sizeof(data), 0);
      deserialize(data_Packet, data);

      std::cout << "| ===" << std::endl;
      std::cout << "Complete" << std::endl;
      std::cout << "Received ID: " << data_Packet->id << std::endl;
      std::cout << "Received Data: " << data_Packet->DoubleValues[0] << std::endl;

    }
  };


  class Client : public Serialization {
   public:
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
        std::cout << "Connection Failing" << std::endl;
        // return;
      }

      dataPacket *data_Packet = new dataPacket;
      std::string ID = "testID";
      strcpy(data_Packet->id, ID.c_str());

      data_Packet->DoubleValues[0] = 2;

      char data[PACKETSIZE];
      serialize(data_Packet, data);
      send(sock, &data, sizeof(data), 0);

      std::cout << "Client data sent" << std::endl;
    }
  };


  class Network : public Server, public Client {
    public:
    static void test() {
      std::thread server_t(server);
      std::thread client_t(client);

      server_t.join();
      client_t.join();
    }    
  };
}


#endif // !CJ_NETWORK
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

#define CJ_DEFAULT_PORT 13200
#define CJ_BUFFSIZE 512

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
      char id[CJ_BUFFSIZE];
      int IntegerValues[CJ_BUFFSIZE];
      bool BooleanValues[CJ_BUFFSIZE];
      double DoubleValues[CJ_BUFFSIZE];
      std::string StringValues[CJ_BUFFSIZE];
      #ifdef SEND_IMAGE
      Image ImageValues[CJ_BUFFSIZE];
      #endif
    };

    #define PACKETSIZE sizeof(dataPacket)

   private:

    /**
     * Serializaiton
     */
    class Serialization {
      protected:
      static void serialize(dataPacket *msgPacket, char *data) {
        int *q = (int*)data;
        *q = msgPacket->DoubleValues[0]; q++;
        char *p = (char*)q;
        int i = 0;
        while (i < CJ_BUFFSIZE) {
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
        while (i < CJ_BUFFSIZE) {
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
     protected:
      struct vals_s {
        int server_fd, new_socket, valread;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        char buffer[1024] = {0};
      };

      static void server(vals_s *vs) {

        // Creating socket file descriptor
        if ((vs->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
          perror("Socket failed");
          return;
        }

        if (setsockopt(vs->server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &vs->opt, sizeof(vs->opt))) {
          std::cout << "setsockopt fail" << std::endl;
          return;
        }

        vs->address.sin_family = AF_INET;
        vs->address.sin_addr.s_addr = INADDR_ANY;
        vs->address.sin_port = htons(CJ_DEFAULT_PORT);

        if (bind(vs->server_fd, (struct sockaddr *)&vs->address, sizeof(vs->address))<0) {
          std::cout << "bind failed";
          return;
        }

        if (listen(vs->server_fd, 3) < 0) {
          std::cout << "listen failed";
        }

        if ((vs->new_socket = accept(vs->server_fd, (struct sockaddr *)&vs->address, (socklen_t*)&vs->addrlen))<0) {
          std::cout << "accept failed";
        }

        dataPacket *data_Packet = new dataPacket;
        int size;
        std::string item_id;
        int iten_value;

        char data[PACKETSIZE];
        size = recv(vs->new_socket, &data, sizeof(data), 0);
        deserialize(data_Packet, data);

        std::cout << "Received ID: " << data_Packet->id << std::endl;
        std::cout << "Received Data: " << data_Packet->DoubleValues[0] << std::endl;
      }
    };


    class Client : public Serialization {
     protected:
      struct vals_c {
        uint16_t port;
        const char *ipaddress = "127.0.0.1";
        int sock = 0;
        int valread;
        char buffer[1024] = {0};
        struct sockaddr_in serv_addr;
      };

      static void _init(vals_c *vs) {
        if ((vs->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
          std::cout << "Socket Creation error" << std::endl;
          return;
        }

        vs->serv_addr.sin_family = AF_INET;
        vs->serv_addr.sin_port = htons(CJ_DEFAULT_PORT);

        if (inet_pton(AF_INET, vs->ipaddress, &vs->serv_addr.sin_addr) <= 0) {
          std::cout << "Invalid IP adress" << std::endl;
          return;
        }

        while(connect(vs->sock, (struct sockaddr *) &vs->serv_addr, sizeof(vs->serv_addr)) < 0) {
          std::cout << "Connection Failing" << std::endl;
        }
      }

      static void _send(vals_c *vs) {

        dataPacket *data_Packet = new dataPacket;
        std::string ID = "testID";
        strcpy(data_Packet->id, ID.c_str());

        data_Packet->DoubleValues[0] = 2;

        char data[PACKETSIZE];
        serialize(data_Packet, data);
        send(vs->sock, &data, sizeof(data), 0);

        std::cout << "Client data sent" << std::endl;
      }
    };

   public:
    class Control : public Server, public Client {
     public:
      struct server {
        static void init();
        static void receive();
        static void send();

        struct Set {
          static void port();
        };
      };

      struct client {
        static void init();
        static void receive();
        static void send();

        struct Set {
          static void port();
          static void ip();
        };
      };

      static void test() {
        std::cout << "Testing CJ Network" << std::endl;
        vals_s vss;
        vals_c vsc;
        std::thread server_t(Server::server, &vss);
        Client::_init(&vsc);
        Client::_send(&vsc);

        server_t.join();
      }
    };
  };
}


#endif // !CJ_NETWORK
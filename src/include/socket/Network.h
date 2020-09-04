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

#define CJ_DEFAULT_PORT 3000
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

    struct vals_c {
      uint16_t port;
      const char *ipaddress = "127.0.0.1";
      int sock = 0;
      int valread;
      char buffer[1024] = {0};
      struct sockaddr_in serv_addr;
    };

    struct vals_s {
      int server_fd, new_socket, valread;
      int opt = 1;
      struct sockaddr_in address;
      int addrlen = sizeof(address);
      char buffer[1024] = {0};
    };


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
      static void _init(vals_s *vs) {
        std::cout << "Server Init Start" << std::endl;

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

        std::cout << "Socket created" << std::endl;

        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
          std::cout << "setsockopt fail" << std::endl;
          return;
        }

        std::cout << "set socket successful" << std::endl;

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(CJ_DEFAULT_PORT);

        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
          std::cout << "bind failed";
          return;
        }

        std::cout << "Bind success" << std::endl;

        if (listen(server_fd, 3) < 0) {
          std::cout << "listen failed";
        }

        std::cout << "Listen success" << std::endl;

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
          std::cout << "accept failed";
        }

        std::cout << "Server Creation Sucessful" << std::endl;
      }

      static void _send(vals_s *vs, dataPacket *dataPack) {
        char data[PACKETSIZE];
        serialize(dataPack, data);
        send(vs->new_socket, &data, sizeof(data), 0);
      }

      static void _reveive(vals_s *vs, dataPacket *dataPack) {
        int size;
        char data[PACKETSIZE];
        size = recv(vs->new_socket, &data, sizeof(data), 0);
        deserialize(dataPack, data);

        std::cout << "Received ID: " << dataPack->id << std::endl;
        std::cout << "Received Data: " << dataPack->DoubleValues[0] << std::endl;
      }
    };


    class Client : public Serialization {
     protected:

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
        std::cout << "Connection Successful" << std::endl;
      }

      static void _send(vals_c *vs, dataPacket *dataPack) {
        char data[PACKETSIZE];
        serialize(dataPack, data);
        send(vs->sock, &data, sizeof(data), 0);
      }

      static void _receive(vals_c *vs, dataPacket *dataPack) {
        int size;
        char data[PACKETSIZE];
        size = recv(vs->sock, &data, sizeof(data), 0);
        deserialize(dataPack, data);

        std::cout << "Received ID: " << dataPack->id << std::endl;
        std::cout << "Received Data: " << dataPack->DoubleValues[0] << std::endl;
      }

      static void _registerReceive(vals_c *vs, dataPacket *dataPack) {
        int size;
        char data[PACKETSIZE];
        size = recv(vs->sock, &data, sizeof(data), 0);
        deserialize(dataPack, data);

        std::cout << "Received ID: " << dataPack->id << std::endl;
        std::cout << "Received Data: " << dataPack->DoubleValues[0] << std::endl;
      }
    };

   public:
    class Control : public Server, public Client {
     public:
      struct server : public Server {
       protected:
        vals_s vs;
       public:
        
        void init() {
          std::thread sv_init_t(_init, &vs);
          sv_init_t.detach();
          // _init(vs);
        }

        void registerReceive() {
          
        }
        void registerSend();
        void receive() {

        }
        void send(dataPacket *dataPack) {
          // _send(&vss_, dataPack);
        }

        struct Set {
          static void port();
        };
      };

      struct client : public Client {
       protected:
        vals_c vs;
       public:
        void init() {
          std::thread cl_init_t(_init, &vs);
          cl_init_t.detach();
          // _init(vs);
        }

        void registerReceive(dataPacket *dataPack) {
          // std::thread registerReceive_t(_registerReceive, &vsc, dataPack);
          // registerReceive_t.join();
        }

        void registerSend(dataPacket *dataPack) {
          // std::thread registerSend_t(_send, &vsc, dataPack);
        }

        void receive(dataPacket *dataPack) {
          // _receive(&vsc, dataPack);
        }

        void send(dataPacket *dataPack) {
          // _send(&vsc, dataPack);
        }

        struct Set {
          static void port();
          static void ip();
        };
      };

     protected:
        client cl;
        server sv;
     public:

      void test() {
        std::cout << "Testing CJ Network" << std::endl;
      
        // Sending package
        dataPacket dpSend;
        dpSend.DoubleValues[0] = 3;

        // Getting package
        dataPacket dpGet;

        cl.init();
        sv.init();
        

        


        // s.send(&dpSend);
        // c.registerReceive(&dpGet);
        
        std::cout << "Values from dpGet: " << dpGet.DoubleValues[0] << std::endl; 
      }
    };
  };
}


#endif // !CJ_NETWORK
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
      bool dataTrue = false;
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
      uint16_t port;
      const char *ipaddress = "127.0.0.1";
      int sock = 0;
      int valread;
      struct sockaddr_in serv_addr;
    };

    struct vals_s {
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

        // Double Values
        double *dblVals = (double*)dtTrue;
        int i = 0;
        while (i < CJ_BUFFSIZE) {
          *dblVals = msgPacket->DoubleValues[i];
          dblVals++;
          i++;
        }

        // ID's
        char *id = (char*)dblVals;
        i = 0;
        while (i < CJ_BUFFSIZE) {
          *id = msgPacket->id[i];
          id++;
          i++;
        }
      }

      static void deserialize(dataPacket *msgPacket, char *data) {
        // Data True
        bool *dtTrue = (bool*)data;
        msgPacket->dataTrue = *dtTrue; dtTrue++;

        // Double Values
        double *dblVals = (double*)dtTrue;
        int i = 0;
        while (i < CJ_BUFFSIZE) {
          msgPacket->DoubleValues[i] = *dblVals;
          dblVals++;
          i++;
        }

        // ID's
        char *id = (char*)dblVals;
        i = 0;
        while (i < CJ_BUFFSIZE) {
          msgPacket->id[i] = *id;
          id++;
          i++;
        }
      }
    };


    /**
     * Server Controller
     */
    class Server : public Serialization {
     protected:
      static void _init(vals_s *vs, statesController *stc) {
        stc->setState(statesController::state::CONNECTING);
        std::cout << "Server Init Start" << std::endl;

        int server_fd, new_socket, valread; 
        struct sockaddr_in address; 
        int opt = 1; 
        int addrlen = sizeof(address); 

        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
          perror("Socket failed");
          stc->setState(statesController::state::ERROR);
          return;
        }

        std::cout << "Socket created" << std::endl;

        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
          std::cout << "setsockopt fail" << std::endl;
          stc->setState(statesController::state::ERROR);
          return;
        }

        std::cout << "set socket successful" << std::endl;

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(CJ_DEFAULT_PORT);

        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
          std::cout << "bind failed";
          stc->setState(statesController::state::ERROR);
          return;
        }

        std::cout << "Bind success" << std::endl;

        if (listen(server_fd, 3) < 0) {
          stc->setState(statesController::state::ERROR);
          std::cout << "listen failed";
        }

        std::cout << "Listen success" << std::endl;

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
          std::cout << "accept failed";
          stc->setState(statesController::state::ERROR);
        }

        // Set values
        vs->address = address;
        vs->addrlen = addrlen;
        vs->new_socket = new_socket;
        vs->opt = opt;
        vs->server_fd = server_fd;
        vs->valread = valread;

        std::cout << "Server Creation Sucessful" << std::endl;
        stc->setState(statesController::state::CONNECTED);
      }

      static void _send(vals_s *vs, dataPacket *dataPack) {
        dataPack->dataTrue = true;
        char data[PACKETSIZE];
        serialize(dataPack, data);
        send(vs->new_socket, &data, sizeof(data), 0);
      }

      static void _registerSend(vals_s *vs, dataPacket *dataPack) {
        char data[PACKETSIZE];
        while (true) {
          dataPack->dataTrue = true;
          serialize(dataPack, data);
          send(vs->new_socket, &data, sizeof(data), 0);
        }
      }

      static void _reveive(vals_s *vs, dataPacket *dataPack) {
        int size;
        char data[PACKETSIZE];
        size = recv(vs->new_socket, &data, sizeof(data), 0);
        deserialize(dataPack, data);
      }

      static void _registerReceive(vals_s *vs, dataPacket *dataPack) {
        int size;
        char data[PACKETSIZE];
        while (true) {
          size = recv(vs->new_socket, &data, sizeof(data), 0);
          deserialize(dataPack, data);
        }
      }
    };


    class Client : public Serialization {
     protected:

      static void _init(vals_c *vs, statesController *stc) {
        stc->setState(statesController::state::CONNECTING);
        if ((vs->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
          std::cout << "Socket Creation error" << std::endl;
          stc->setState(statesController::state::ERROR);
          return;
        }

        vs->serv_addr.sin_family = AF_INET;
        vs->serv_addr.sin_port = htons(CJ_DEFAULT_PORT);

        if (inet_pton(AF_INET, vs->ipaddress, &vs->serv_addr.sin_addr) <= 0) {
          std::cout << "Invalid IP adress" << std::endl;
          stc->setState(statesController::state::ERROR);
          return;
        }

        while(connect(vs->sock, (struct sockaddr *) &vs->serv_addr, sizeof(vs->serv_addr)) < 0) {
          std::cout << "Connection Failing" << std::endl;
          stc->setState(statesController::state::ERROR);
        }

        std::cout << "Connection Successful" << std::endl;
        stc->setState(statesController::state::CONNECTED);
      }

      static void _send(vals_c *vs, dataPacket *dataPack) {
        dataPack->dataTrue = true;
        char data[PACKETSIZE];
        serialize(dataPack, data);
        send(vs->sock, &data, sizeof(data), 0);
      }

      static void _registerSend(vals_c *vs, dataPacket *dataPack) {
        dataPack->dataTrue = true;
      }

      static void _receive(vals_c *vs, dataPacket *dataPack) {
        int size;
        char data[PACKETSIZE];
        size = recv(vs->sock, &data, sizeof(data), 0);
        deserialize(dataPack, data);
      }

      static void _registerReceive(vals_c *vs, dataPacket *dataPack) {
        dataPacket dp;
        while (true) {
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
          }
        }
      }
    };

   public:
    class Control : public Server, public Client {
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

        void receive() {

        }

        void registerReceive() {
          
        }

        void send(dataPacket *dataPack) {
          _send(&vs, dataPack);
        }

        void registerSend(dataPacket *dataPack) {
          std::thread registerSend_t(_registerSend, &vs, dataPack);
          registerSend_t.detach();
        }

        struct Set {
          static void port();
        };
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

        void registerReceive(dataPacket *dataPack) {
          std::thread registerReceive_t(_registerReceive, &vs, dataPack);
          registerReceive_t.detach();
        }

        void registerSend(dataPacket *dataPack) {
          // std::thread registerSend_t(_send, &vs, dataPack);
        }

        void receive(dataPacket *dataPack) {
          _receive(&vs, dataPack);
        }

        void send(dataPacket *dataPack) {
          _send(&vs, dataPack);
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

        // State Controller
        network_stc.setState(statesController::state::CONNECTING);

      
        // Sending package
        dataPacket dpSend;
        dpSend.DoubleValues[0] = 3.56;

        // Getting package
        dataPacket dpGet;

        cl.init();
        sv.init();

        while (network_stc.getState() != statesController::state::CONNECTED) {
          if (cl.getState() != statesController::state::CONNECTED && sv.getState() != statesController::state::CONNECTED) {
            std::cout << "Client State: " << cl.getState() << std::endl;
            std::cout << "Server State: " << sv.getState() << std::endl;
            std::cout << "Main State: " << network_stc.getState() << std::endl;
          } else if (cl.getState() == statesController::state::CONNECTED && sv.getState() == statesController::state::CONNECTED) {
            network_stc.setState(statesController::CONNECTED);
            std::cout << "Main State: " << network_stc.getState() << std::endl;
          }
        }

        sv.registerSend(&dpSend);
        cl.registerReceive(&dpGet);

        while (true) {
          for (double i = 0; i < 10000; i++) {
            dpSend.DoubleValues[0] += 0.001;
            std::cout << "Values from dpGet: " << dpGet.DoubleValues[0] << std::endl; 
          } 
          for (double i = 0; i < 10000; i++) {
            dpSend.DoubleValues[0] -= 0.001;
            std::cout << "Values from dpGet: " << dpGet.DoubleValues[0] << std::endl; 
          }
        }
      }
    };
  };
}


#endif // !CJ_NETWORK
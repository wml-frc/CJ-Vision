#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#define DEFAULT_PORT 3000

namespace CJ {
  class Network {
   public:
    static void Send(cv::Mat *image) { 
      std::cout << "Server start" << std::endl;
      int server_fd, new_socket, valread; 
      struct sockaddr_in address; 
      int opt = 1; 
      int addrlen = sizeof(address); 
      char buffer[1024] = {0}; 
      const char *hello = "Hello from server"; 
      
      // Creating socket file descriptor 
      if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
      } 

      std::cout << "socket made Server" << std::endl;
      
      // Forcefully attaching socket to the default port 
      if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                            &opt, sizeof(opt))) { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
      } 
      address.sin_family = AF_INET; 
      address.sin_addr.s_addr = INADDR_ANY; 
      address.sin_port = htons( DEFAULT_PORT ); 
      
      // Forcefully attaching socket to the default port 
      if (bind(server_fd, (struct sockaddr *)&address, 
                    sizeof(address))<0) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
      } 
      if (listen(server_fd, 3) < 0) { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
      } 
      if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
              (socklen_t*)&addrlen))<0) { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
      } 


      
      *image = (image->reshape(0,1));
      int imgsize = image->total()*image->elemSize();
      // send(sock , hello , strlen(hello) , 0 ); 
      ssize_t bytes = send(new_socket, image->data, imgsize, 0);
      printf("Image Sent\n"); 
      return; 
    } 






    // Client side C/C++ program to demonstrate Socket programming 

    static void Receive(cv::Mat *image) { 
      std::cout << "Start client" << std::endl;
      int sock = 0, valread; 
      struct sockaddr_in serv_addr; 
      const char *hello = "Hello from client"; 
      char buffer[1024] = {0}; 
      if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        printf("\n Socket creation error \n"); 
        return; 
      } 

      serv_addr.sin_family = AF_INET; 
      serv_addr.sin_port = htons(DEFAULT_PORT); 

      std::cout << "Socket made Client" << std::endl;
      
      // Convert IPv4 and IPv6 addresses from text to binary form 
      if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) { 
        printf("\nInvalid address/ Address not supported \n"); 
        return; 
      } 

      if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        printf("\nConnection Failed \n"); 
        return; 
      } 
      *image = cv::Mat::zeros(480, 640, CV_8UC3);
      int imgSize = image->total()*image->elemSize();
      uchar sockData[imgSize];

      ssize_t bytes;
      for (int i = 0; i < imgSize; i += bytes) {
        if ((bytes = recv(sock, sockData +i, imgSize - i, 0)) == -1) {
          std::cout << "recv failed" << std::endl;
        }
      }
      

      int ptr=0;
      for (int i = 0; i < image->rows; i++) {
        for (int j = 0; j < image->cols; j++) {
          image->at<cv::Vec3b>(i,j) = cv::Vec3b(sockData[ptr+0], sockData[ptr+1], sockData[ptr+2]);
          ptr=ptr+3;
        }
      }

      printf("Image Received\n"); 
      return; 
    } 
  };
}
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <thread>
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <iostream>
#define DEFAULT_PORT 3000

namespace CJ {
  class Network {
   public:
    static void Send(cv::VideoCapture *cap) {
      
      // Socket Setup
      int localSocket,
          remoteSocket,
          port = DEFAULT_PORT,
          opt = 1;
      
      struct sockaddr_in localAddr,
                         remoteAddr;

      pthread_t thread_id;

      int addrLen = sizeof(struct sockaddr_in);
      
      if ((localSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket Failed");
      }

      if (setsockopt(localSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("SetSockopt Failed");
      }

      localAddr.sin_family = AF_INET;
      localAddr.sin_addr.s_addr = INADDR_ANY;
      localAddr.sin_port = htons(port);

      if (bind(localSocket, (struct sockaddr *)&localAddr, sizeof(localAddr)) < 0) {
        perror("Can't bind() socket");
      }

      // Listen
      listen(localSocket, 3);
      std::cout << "Waiting for connections...\n";
      std::cout << "Port: " << port << std::endl;
      

      while (1) {
        remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);
        if (remoteSocket < 0) {
          perror("accept failed");
          break;
        }

        printf("Connection Accepted\n");

        // pthread_create(&thread_id, NULL, display, &remoteSocket);
        std::thread display_t(display, &remoteSocket, cap);
      }
    }
    static void Receive() {

    }



   private:
    static void *display(void *ptr, cv::VideoCapture *cap) {
      int socket = *(int *)ptr;

      // OpenCV Image 
      cv::Mat image, imageGray;
      image = cv::Mat::zeros(480, 640, CV_8UC1);
      // Make continuous
      if (!image.isContinuous()) {
        image = image.clone();
      }

      int imgSize = image.total() * image.elemSize();
      int bytes = 0;
      int key;

      // Make image continuous
      if (!image.isContinuous()) {
        image = image.clone();
        imageGray = image.clone();
      }

      std::cout << "Image size: " << imgSize << std::endl;

      while (1) {
        *cap >> image;

        cv::cvtColor(image, imageGray, CV_BGR2GRAY);

        // Send Processed Image
        if ((bytes = send(socket, imageGray.data, imgSize, 0)) < 0) {
          std::cerr << "bytes = " << bytes << std::endl;
          break;
        }
      }
    }


  };
}
#ifndef NETHEADER_H
#define NETHEADER_H

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

#define DEFAULT_PORT 13200
#define BUFFSIZE 512

struct testStruct {
  int testInt;
  // bool testBool;
  // std::string testString;
  char id[BUFFSIZE];
};

#define PACKETSIZE sizeof(testStruct)

#endif // !NETHEADER_H
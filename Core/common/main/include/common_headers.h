#ifndef COMMON_HEADERS_H
#define COMMON_HEADERS_H

// System common headers
#include <iostream>
#include <string>

// Network
#ifdef CJ_PLATFORM_LINUX
#include "UDP_TransferNT.h" // UDP is only linux currently
#endif

// CV
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>

// Our common headers
#include "Utils/Utils.h"
#include "ErrorHandles/Assert.h"

#endif
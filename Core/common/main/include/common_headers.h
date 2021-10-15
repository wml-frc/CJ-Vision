#ifndef COMMON_HEADERS_H
#define COMMON_HEADERS_H

// System common headers
#include <iostream>
#include <string>

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
#include "Logging/Print.h"

// override the NT logger with the CJ_CORE_PRINTER
#define DEFAULT_NT_LOGGER(...) CJ_PRINT_WARN(__VA_ARGS__)

#endif
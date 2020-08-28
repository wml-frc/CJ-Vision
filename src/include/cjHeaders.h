#ifndef cjHEADERS_H
#define cjHEADERS_H

class PROG {
 private:
  // MAIN PROG
  static bool PROG_RUNNING_b;

  // THREADING
  static bool THREADS_RUNNING_b;
 public:
  static bool PROG_RUNNING() {return PROG_RUNNING_b;}
  static bool THREADS_RUNNING() {return THREADS_RUNNING_b;}
  static void set_PROG_RUNNING(bool status) {PROG_RUNNING_b = status;}
  static void set_THREADS_RUNNING(bool status) {THREADS_RUNNING_b = status;}
};

// OPENCV
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>

#endif
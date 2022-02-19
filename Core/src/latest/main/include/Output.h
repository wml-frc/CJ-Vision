#ifndef OUTPUT_H
#define OUTPUT_H

#include "ImageContainer.h"

#ifdef CJ_PLATFORM_LINUX
#include "MJPEGWriter.h"
#endif

namespace CJ {

  /**
   * Main output class
   */
  class Output {
  public:

    /**
      * Video Stream class.
      * Outputs MJPEG stream to a port number
      */
    class Stream {
    public:

      /**
        * Create Stream on port number
        */
      Stream(int port) {
        #ifdef CJ_PLATFORM_LINUX
        CJ_CORE_PRINT_INFO("Streamer Active");
        _writer = std::make_shared<MJPEGWriter>(port);
        #else
        CJ_CORE_PRINT_INFO("Not linux platform. Streamer Inactive");
        #endif
      }

      ~Stream() {
        #ifdef CJ_PLATFORM_LINUX
        _writer->release();
        _writer->stop();
        #endif
      }
      
      /**
        * Start stream thread
        */
      void start() {
        #ifdef CJ_PLATFORM_LINUX
        _writer->start();
        #endif
      }

      /**
        * Stop stream thread
        */
      void stop() {
        #ifdef CJ_PLATFORM_LINUX
        _writer->stop();
        #endif
      }

      /**
        * Output image to video stream
        */
      void output(Image image) {
        #ifdef CJ_PLATFORM_LINUX
        _writer->write(image.data);
        #endif
      }

    private:
      #ifdef CJ_PLATFORM_LINUX
      std::shared_ptr<MJPEGWriter> _writer;
      #endif
    };

    /**
      * Input wait in ms, and diplay image/s
      * Returns waitkey output, or returns -1 on error
      */
    static void display() {
      return;
    }

    /**
      * Input wait in ms, and diplay image/s
      * Returns waitkey output, or returns -1 on error
      */
    template<typename T, typename... Args>
    static void display(T image, Args... rest) {
      #ifndef COPROC
      if (image.data.empty()) {
        CJ_CORE_PRINT_ERROR("Display Error: " + image.name + ". Is empty");
      } else {
        cv::imshow(image.name, image.data);
      }

      display(rest...);
      #endif
    }

    /**
      * Input wait in ms, and diplay image/s
      * Returns waitkey output, or returns -1 on error
      */
    template<typename T, typename... Args>
    static int display(int wait_ms, T image, Args... rest) {
      #ifndef COPROC
      if (image.data.empty()) {
        CJ_CORE_PRINT_ERROR("Display Error: " + image.name + ". Is empty");
        return -1;
      } else {
        cv::imshow(image.name, image.data);
      }
      
      display(rest...);

      if (wait_ms != 0) {
        int key = cv::waitKey(wait_ms);
        return key;
      }

      #endif
      return 0;
    }
  };
}

#endif
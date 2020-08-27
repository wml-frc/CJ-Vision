#include <sys/socket.h>
#include <arpa/inet.h>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#define PORT 13200

class Display {
 public:

  Display(cv::Mat image) {

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      printf("\n Socket Creation Error \n");
      return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
      printf("\n Invalid addresses/ Adresses not supported \n");
      return;
    }

    // if (connect(sock, ()))

    // Send image
    image = (image.reshape(0,1));
    int imgSize = image.total()*image.elemSize();
    int bytes = send(sock, image.data, imgSize, 0);
  }

 private:
  struct sockaddr_in serv_addr;
  int sock = 0, valread;
};
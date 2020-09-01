#include "NetHeader.h"

namespace CJ {
  class Receiver {
   public:
    static void Receive() {
      int sock;
      const char *serverIP;
      int serverPort;

      serverIP = "127.0.0.1";
      serverPort = DEFAULT_PORT;

      struct sockaddr_in serverAddr;
      socklen_t addrLen = sizeof(struct sockaddr_in);

      if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket() failed" << std::endl;
      }

      serverAddr.sin_family = PF_INET;
      serverAddr.sin_addr.s_addr = inet_addr(serverIP);
      serverAddr.sin_port = htons(serverPort);

      // if (connect(sock, (sockaddr*)&serverAddr))
    }
  };
}
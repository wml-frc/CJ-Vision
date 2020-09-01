#include "Client.h"
#include "Server.h"

#include <thread>

namespace CJ {
  class Network : public Server, public Client {
    public:
    static void test() {
      std::thread server_t(server);
      std::thread client_t(client);

      server_t.join();
      client_t.join();
    }    
  };
}


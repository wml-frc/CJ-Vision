#include "Receive.h"
#include "Send.h"

namespace CJ {
  class Network : public Sender, public Receiver {};
}
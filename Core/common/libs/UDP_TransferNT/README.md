# UDP Transfer Network

## Fast paced point to point network connection

### Project Overview
- UDP (User Datagram Protocol) does not require the need for handshaking between a server and client. Due to this it is faster than other network transfer protocols like TCP. This network library is a small but usefull wrapper around both winsock2 and the standard unix socket for C programs, written in C++. Providing methods for creating both Servers and Clients with the option of either allowing ANY connection or IP_SPECIFIC connections for point to point transfer.

- Sending and Receiving can be done either using the raw send receive functions with a byte array, or by using the built in datapackets which are serialized and deserialized using the the Serializer. The project has a default buffersize of `256` bytes which can be overridden. 

- Datapackets are sent using a serializing method of 4 data type arrays that fit into the buffer. Characters, Integers, Booleans and Decimals. The data type arrays have their own segmented block inside the main byte array which is sent and received. These segments (TYPEBLOCKS) vary in size but have an even amount of bytes alocated to them. I.e. A buffersize of `256 bytes` has segmented blocks that resemble `|64|64|64|64|`. Each block has `64 bytes` allocated to it. But could also be represented as `|Characters|Integers|Booleans|Decimals|`. Where each segmented block is a data type array of either characters, integers, booleans or decimals. You can see the method of serializing and deserializing in the file [Serializer.h](UDP_TransferNT/include/Serializer.h).

- Each segment block varies in how many values it can hold. Because the buffersize can change depending on what the user defines it as. I.e, if the buffersize is 256, each block size is 64 bytes. Then you could hold `64 characters`, `16 integers`, `64 booleans` and `16 decimals` (which are stored as floats). The array of data is specified as `dataType DataArray[BLOCK_SIZE/sizeof(dataType)]`.

### Installation
- The project is built as a header library and just needs the `include` dir linked with the build of your main project

- To install, you can either download the project and place the `include` directory in your project. Or clone the project and use it as a submodule, which you can then link the `include` directory via your build method.

#### FRC Install
- For FRC Teams the project can be linked simply through `build.gradle`. To do this, append your build.gradle to include the location of the `include` directory. This will link the header files and build them when you run `./gradlew build`. 
```gradle
sources.cpp {
		source {
			srcDir 'src/main/cpp'
			include '**/*.cpp', '**/*.cc'
		}
		exportedHeaders {
			srcDirs ["src/main/include", "<UDP_TRASNFERNT_LOCATION>/UDP_TransferNT/include"]
			if (includeSrcInIncludeRoot) {
					srcDir 'src/main/cpp'
			}
		}
	}
```

- Note, that if you are a team which uses [WML](https://github.com/wml-frc/WML), then this library should already be linked up.



# Example Usage

### Point to Point with datapackets

#### Config
- Server is our `receiver` | IP_Specific/ANY
- Client is our `sender` | IP_Specific
- Port: `5801` <- Default
- IP: `10.47.88.2` Default is -> localhost: `127.0.0.1`
- Buffersize: `256` <- Default
- RecvTimeout: `1000` <- default (ms)

#### Server (Receiver)
```cpp
#define DEFAULT_BUFFER_SIZE 256 // default if not set
#include "UDP_TransferNT.h"

using namespace UDP_TransferNT;
Network s_network(Network::Type::SERVER, Network::ConnectionType::IP_SPECIFIC);

int main() {
	s_network.getSocket().setRecvTimeout(1000); // Must be done before network init
	s_network.init();

	DataPacket data;

	while (true) {
		data = s_network.dpRecv(data); // will stop program and wait until it either gets data. Or times out (see setRecvTimeout(ms))

		std::cout << "DP Recv Character at index 0: " << data.getCharacters(0) << std::endl;
		std::cout << "DP Recv Integer at index 3: " << data.getIntegers(3) << std::endl;
		std::cout << "DP Recv Boolean at index 5: " << data.getBooleans(5) << std::endl;
		std::cout << "DP Recv Decimal at index 0: " << data.getDecimals(0) << std::endl;

		#ifdef NT_UDP_PLATFORM_WINDOWS
		Sleep(1000);
		#elif defined(NT_UDP_PLATFORM_UNIX)
		usleep(1000*1000); // um
		#endif
	}

	return 0;
}
```

#### Client (Sender)
```cpp
#define DEFAULT_BUFFER_SIZE 256 // default if not set
#include "UDP_TransferNT.h"

using namespace UDP_TransferNT;
Network c_network(Network::Type::CLIENT, Network::ConnectionType::IP_SPECIFIC);

int main() {
	c_network.getSocket().setIP("10.47.88.2")
	c_network.getSocket().setPort(5801); // default if not set
	c_network.init();

	DataPacket data;
	float decimalValue = 0.1;
	while (true) {
		data.setCharacters(0, 'c');
		data.setIntegers(3, 3);
		data.setBooleans(5, true);
		data.setDecimals(0, decimalValue);

		decimalValue += 0.1;

		c_network.dpSend(data); // sends the data (does not stop program)

		#ifdef NT_UDP_PLATFORM_WINDOWS
		Sleep(1000);
		#elif defined(NT_UDP_PLATFORM_UNIX)
		usleep(1000*1000); // um
		#endif
	}

	return 0;
}
```

### Point to Point using raw buffers
- If the preference of using a raw send receive without a serialized datapacket is more appealing. The raw sender and receivers are available to use. Similar to the datapackets, they fit inside the the set buffer size. But do not segment the buffer at all. If you plan to use your own method of serializing then this would be preferable. E.g, you could send `256 characters` over the network if you wanted a full string message to be sent.

#### Config
- Server is our `receiver` | IP_Specific/ANY
- Client is our `sender` | IP_Specific
- Port: `5801` <- Default
- IP: `10.47.88.2` Default is -> localhost: `127.0.0.1`
- Buffersize: `256` <- Default
- RecvTimeout: `1000` <- default (ms)

#### Server (Receiver)
```cpp
#define DEFAULT_BUFFER_SIZE 256 // default if not set
#include "UDP_TransferNT.h"

using namespace UDP_TransferNT;
Network s_network(Network::Type::SERVER, Network::ConnectionType::IP_SPECIFIC);

int main() {
	s_network.getSocket().setRecvTimeout(1000); // Must be done before network init
	s_network.init();

	char *rawBuffer = (char *)malloc(DEFAULT_BUFFER_SIZE * sizeof(char)); // Malloc a byte array to the size of the buffer

	while (true) {
		int recvVal = 0;
		if ((recvVal = s_network.raw_recv(rawBuffer)) != 0) {
			std::cout << "Receive error: " recvVal << std::endl;
		} else {
			std::cout << "Messafe Received: " << rawBuffer << std::endl;
		}

		#ifdef NT_UDP_PLATFORM_WINDOWS
		Sleep(1000);
		#elif defined(NT_UDP_PLATFORM_UNIX)
		usleep(1000*1000); // um
		#endif
	}

	free(rawBuffer); // free the buffer to stop memory leaking
	return 0;
}
```


#### Client (Sender)
```cpp
#define DEFAULT_BUFFER_SIZE 256 // default if not set
#include "UDP_TransferNT.h"

using namespace UDP_TransferNT;
Network c_network(Network::Type::CLIENT, Network::ConnectionType::IP_SPECIFIC);

int main() {
	c_network.getSocket().setIP("10.47.88.2")
	c_network.getSocket().setPort(5801); // default if not set
	c_network.init();

	char rawBuffer[DEFAULT_BUFFER_SIZE] = {"Hello from client"}; // create the buffer

	while (true) {
		int sendVal = 0;
		if ((sendVal = c_network.raw_send(rawBuffer)) != 0) {
			std::cout << "Send error: " << sendVal << std::endl;
		} else {
			std::cout << "Message sent" << std::endl;
		}

		#ifdef NT_UDP_PLATFORM_WINDOWS
		Sleep(1000);
		#elif defined(NT_UDP_PLATFORM_UNIX)
		usleep(1000*1000); // um
		#endif
	}

	return 0;
}
```



### Notes and Optimisation
- If this network is being used for fast paced networking between two or more devices you can optimise the project heavily. However I caution anyone doing it. As this uses raw socket programming compared to other methods (i.e Network Tables). It can easily saturate an entire existing network, as it has no speed limit unless specified to do so. The only limitation, is the hardware this project runs on, and the program that it's being imported into. It's recommended to slow this project down to a cycle time of 10ms. As this provides both a decent speed and relatively low network saturation cost.

- That being said. To optimise the project there are two main methods to do so. On the receiving end the project receive timeout can be lowered to a minimum of 1 (Meaning 1 ms). This results in the program instantly skipping the `dpRecv()` or `raw_recv()` functions unless data exists. Because of this, a receive error will often pop up, especially if one device is running faster than the other. 

- This can be resolved by disabling the logger before including the library. This improves speed as the logger requires some logic to output text to the console. However, it will remove error messages too.

- An example of this type of optimisation can be seen below (Sender side needs no real change other than removing any function to make the project wait/sleep)

#### Server (Receiver)
```cpp
#define DEFAULT_BUFFER_SIZE 256 // default if not set
#define DISABLE_NT_LOGGER
#include "UDP_TransferNT.h"

using namespace UDP_TransferNT;
Network s_network(Network::Type::SERVER, Network::ConnectionType::IP_SPECIFIC);

int main() {
	s_network.getSocket().setRecvTimeout(1); // Must be done before network init
	s_network.init();

	DataPacket data;

	while (true) {
		data = s_network.dpRecv(data); // will stop program and wait until it either gets data. Or times out (see setRecvTimeout(ms))

		std::cout << "DP Recv Character at index 0: " << data.getCharacters(0) << std::endl;
		std::cout << "DP Recv Integer at index 3: " << data.getIntegers(3) << std::endl;
		std::cout << "DP Recv Boolean at index 5: " << data.getBooleans(5) << std::endl;
		std::cout << "DP Recv Decimal at index 0: " << data.getDecimals(0) << std::endl;
	}

	return 0;
}
```

#### Client (Sender)
```cpp
#define DEFAULT_BUFFER_SIZE 256 // default if not set
#define DISABLE_NT_LOGGER
#include "UDP_TransferNT.h"

using namespace UDP_TransferNT;
Network c_network(Network::Type::CLIENT, Network::ConnectionType::IP_SPECIFIC);

int main() {
	c_network.getSocket().setIP("10.47.88.2")
	c_network.getSocket().setPort(5801); // default if not set
	c_network.init();

	DataPacket data;
	float decimalValue = 0.1;
	while (true) {
		data.setCharacters(0, 'c');
		data.setIntegers(3, 3);
		data.setBooleans(5, true);
		data.setDecimals(0, decimalValue);

		decimalValue += 0.1;

		c_network.dpSend(data); // sends the data (does not stop program)
		#endif
	}

	return 0;
}
```

- The above disables the logger to stop any output from the library itself. And lowers the timeout from 1s to 1ms. The sender as well has no lib output from the logger. And also has no function to make it wait.
- The above is the fastest you can feasibly get with this network, other than lowering the buffersize to decrease serialization time. And or using the raw sender and receiver instead which does not need to be serialized/deserialized.



### Overrides and extra functions
- The project comes with a range of customisable features for the user. Many macros can be overridden for better use. As well as many functions to use for different use cases. Below lists the most common.

```cpp
// These macro overrides must be specified before including the network library

/**
 * Disables the logger and any output
 */
#define DISABLE_NT_LOGGER

/**
 * Use a different logger than the internal one. (Currently is DEFAULT_NT_LOGGER(x) std::cout << x << std::endl)
 */
#define DEFAULT_NT_LOGGER(x)

/**
 * Specify the default IP
 * The need for changing is not very high, as the ip can be specified when making the instance of the network. And also changed separately afterwards using the getSocket() function.
 */
#define DEFAULT_NT_IP

/**
 * Specify the default port,
 * likewise with the ip address. It can be changed manually during runtime with getSocket()
 */
#define DEFAULT_NT_PORT

/**
 * Change the default type block number. Changed how many blocks are made inside the buffer. (not recommended to change) as the number of blocks must be able to divide the buffersize evenly. E.g, block num is 4, 256/4 = 64 bytes
 */
#define DEFAULT_N_TYPEBLOCK

/**
 * Specify the buffer size. If you specify a buffer size on one device. You must change it to match on the other device/devices
 */
#define DEFAULT_BUFFER_SIZE


#include "UDP_TransferNT.h" // macros are specified before this include


// An instance of the network is required for all of these functions

/**
 * Network instance, with default options, type, connection type (ANY/IP_SPECIFIC), port, ip
 */
UDP_TransferNT::Network::Network(Type type, ConnectionType connType, int port = DEFAULT_NT_PORT, const char *ip = DEFAULT_NT_IP);

/**
 * Call the deconstructor, kills the socket and all the values
 */
UDP_TransferNT::Network::~Network();

/**
 * Get the socket wrapper
 */
UDP_TransferNT::Socket &UDP_TransferNT::Network::getSocket();

/**
 * Initialize the network with prior defined values (port number, network type, etc...)
 * And connect/bind the server/client
 */
void UDP_TransferNT::Network::init();

/**
 * Resets the network and calls the init function again.
 */
void UDP_TransferNT::Network::reset();

/**
 * Raw send, sends a raw buffer the size of the buffer size
 * Returns 0 or 1 depending on success or failure
 */
int UDP_TransferNT::Network::raw_send(const char buffer[DEFAULT_BUFFER_SIZE]);

/**
 * Receive a raw buffer
 * returns 0 or 1 depending on success or failure
 */
int UDP_TransferNT::Network::raw_recv(char *buffer);

/**
 * Send over a datapacket
 */
void UDP_TransferNT::Network::dpSend(UDP_TransferNT::DataPacket &dp);

/**
 * receive a datapacket from the network.
 * OPTIONAL: put in previously received datapacket to stop zeroed out data packets being returned if the receive fails
 */
UDP_TransferNT::DataPacket UDP_TransferNT::Network::dpRecv(UDP_TransferNT::DataPacket previousDP = {0});





// The socket values need an instance of the network to access. E.g `network.getSocket().<socket function>()`

/**
 * Set the network port
 */
void UDP_TransferNT::Socket::setPort(int port);

/**
 * Set the ip of the network
 */
void UDP_TransferNT::Socket::setIP(const char *ip);

/**
 * Get the currently set port
 */
int UDP_TransferNT::Socket::getPort();

/**
 * Get the currently set ip
 */
const char *UDP_TransferNT::Socket::getIP();

/**
 * Set the receiver timeout (in ms)
 */
void UDP_TransferNT::Socket::setRecvTimeout(int ms);

/**
 * Get the receiver timeout value (in ms)
 */
int UDP_TransferNT::Socket::getRecvTimeout();

/**
 * Get the local address
 */
struct sockaddr_in &UDP_TransferNT::Socket::getLocalAddress();

/**
 * Get the other/external address
 */
struct sockaddr_in &UDP_TransferNT::Socket::getOtherAddress();

/**
 * Get the address length for both local and other
 */
#ifdef NT_UDP_PLATFORM_WINDOWS
int *UDP_TransferNT::Socket::getLocalAddressLength();
int *UDP_TransferNT::Socket::getOtherAddressLength();
#elif defined(NT_UDP_PLATFORM_UNIX)
socklen_t *UDP_TransferNT::Socket::getLocalAddressLength();
socklen_t *UDP_TransferNT::Socket::getOtherAddressLength();
#endif

/**
 * Create the socket from prior defined values
 * Returns 0 on success, 1 on failure
 */
int UDP_TransferNT::Socket::createSocket(bool client = false);

/**
 * Prepare the socket structure.
 * Defines the values needed for client or server. And if it's ANY or IP_SPECIFIC
 */
void UDP_TransferNT::Socket::prepSocketStructure(bool client = false, bool ipSpecific = false);

/**
 * Bind to the socket
 * Used for server to bind and listen for connections
 * 
 * returns 0 or 1 on success/failure
 */
int UDP_TransferNT::Socket::bindSocket(bool client = false);

/**
 * Connect to external address
 * Used for client to connect to other addresses (mainly used in IP_SPECIFIC workloads).
 * This function can be disables for a more open network even with IP_SPECIFIC enabled
 * 
 * returns 0 or 1 on success/failure
 */
int UDP_TransferNT::Socket::connectSocket(bool client = false);

/**
 * Disable the connect function
 */
void UDP_TransferNT::Socket::disableConnect(bool disable = true);

/**
 * Kill the socket and purge any existing values
 */
void UDP_TransferNT::Socket::killSocket();

```

<sub><sup>readme written by [@CJBuchel](https://github.com/CJBuchel), 16/09/21</sup></sub>

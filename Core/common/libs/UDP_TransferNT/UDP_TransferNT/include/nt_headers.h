#ifndef NT_HEADERS_H
#define NT_HEADERS_H

#include "nt_platform.h"
#include <string>
#include <iostream>
#include <typeinfo>

#ifdef DISABLE_NT_LOGGER
	#define DEFAULT_NT_LOGGER(x)
#else
	// override if you have a proper logger. E.g spdlog
	#ifndef DEFAULT_NT_LOGGER
	#define DEFAULT_NT_LOGGER(x) std::cout << x << std::endl
	#endif
#endif

#ifndef DEFAULT_NT_IP
#define DEFAULT_NT_IP "127.0.0.1" // local ip address
#endif

#ifndef DEFAULT_NT_PORT
#define DEFAULT_NT_PORT 5801 // official allowed frc ports are 5800 to 5810
#endif

#ifndef DEFAULT_N_TYPEBLOCK
#define DEFAULT_N_TYPEBLOCK 4
#endif

#ifndef DEFAULT_BUFFER_SIZE
#define DEFAULT_BUFFER_SIZE 256 // must be devisable by typeblock size
#endif

#define DATAPACKET_TYPESIZE DEFAULT_BUFFER_SIZE/DEFAULT_N_TYPEBLOCK

// headers for windows socket
#ifdef NT_UDP_PLATFORM_WINDOWS
	#include <stdio.h>
	#include <WinSock2.h>
	#include <WS2tcpip.h>
	#pragma comment(lib,"Ws2_32.lib")
#elif defined(NT_UDP_PLATFORM_UNIX)
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <arpa/inet.h>
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <unistd.h>
	#include <tuple>
#endif // nt platform
#endif // NT_HEADERS_H
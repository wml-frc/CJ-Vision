#ifndef NT_PLATFORM_H
#define NT_PLATFORM_H

#ifdef _WIN32
	#define NT_UDP_PLATFORM_WINDOWS

#elif defined(__linux__)
	#define NT_UDP_PLATFORM_LINUX

#elif defined (__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>

	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define NT_UDP_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define NT_UDP_PLATFORM_MACOS
	#else
		#error "Unknown Apple platform!"
	#endif

#else
	#error "Unknown platform!"
#endif

#endif

#if defined(NT_UDP_PLATFORM_LINUX) || defined(NT_UDP_PLATFORM_MACOS)
	#define NT_UDP_PLATFORM_UNIX
#endif
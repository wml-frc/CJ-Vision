#ifndef PLATFORM_DETECTION_H
#define PLATFORM_DETECTION_H

#ifdef _WIN32
	#define CJ_PLATFORM_WINDOWS

#elif defined(__linux__)
	#define CJ_PLATFORM_LINUX

#elif defined (__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>

	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define CJ_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define CJ_PLATFORM_MACOS
	#else
		#error "Unknown Apple platform!"
	#endif

#else
	#error "Unknown platform!"
#endif

#endif
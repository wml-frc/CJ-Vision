#ifndef UTILS_H
#define UTILS_H

#include <memory>

#ifdef DEBUG_CJ
	#if defined(CJ_PLATFORM_WINDOWS)
		#define CJ_DEBUGBREAK() __debugbreak()
	#elif defined(CJ_PLATFORM_LINUX)
		#include "signal.h"
		#define CJ_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform does not support debugbreak yet"
	#endif
#else
	#define CJ_DEBUGBREAK()
#endif

#define CJ_EXPAND_MACRO(x) x
#define CJ_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#endif
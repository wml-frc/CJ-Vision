#ifndef ASSERT_H
#define ASSERT_H

// filesystem is as absolute pain. C++ std is finicky between platforms and the switch between <experimental/filesystem> & <filesystem>

#if __has_include(<filesystem>)
	#include <filesystem>
	#define cj_filesystem std::filesystem;
#elif __has_include(<experimental/filesystem>)
	#include <experimental/filesystem>
	#define cj_filesystem std::experimental::filesystem;
#else
	// without filesystem the program will break. So disable asserts
	#define CJ_DISABLE_ASSERTS
#endif

#include "Logging/Print.h"

#ifndef CJ_DISABLE_ASSERTS
	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define CJ_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { CJ##type##PRINT_ERROR(msg, __VA_ARGS__); CJ_DEBUGBREAK(); } }
	#define CJ_INTERNAL_ASSERT_WITH_MSG(type, check, ...) CJ_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define CJ_INTERNAL_ASSERT_NO_MSG(type, check) CJ_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", CJ_STRINGIFY_MACRO(check), cj_filesystem::path(__FILE__).filename().string(), __LINE__)

	#define CJ_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define CJ_INTERNAL_ASSERT_GET_MACRO(...) CJ_EXPAND_MACRO( CJ_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, CJ_INTERNAL_ASSERT_WITH_MSG, CJ_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define CJ_ASSERT(...) CJ_EXPAND_MACRO( CJ_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define CJ_CORE_ASSERT(...) CJ_EXPAND_MACRO( CJ_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define CJ_ASSERT(...)
	#define CJ_CORE_ASSERT(...)
#endif

#endif
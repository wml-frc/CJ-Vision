#ifndef ASSERT_H
#define ASSERT_H

#include "Logging/Print.h"
#include <filesystem>

#ifndef CJ_DISABLE_ASSERTS
	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define CJ_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { CJ##type##PRINT_ERROR(msg, __VA_ARGS__); CJ_DEBUGBREAK(); } }
	#define CJ_INTERNAL_ASSERT_WITH_MSG(type, check, ...) CJ_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define CJ_INTERNAL_ASSERT_NO_MSG(type, check) CJ_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", CJ_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

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
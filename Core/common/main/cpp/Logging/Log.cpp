#include "Logging/Log.h"

namespace CJ {
	std::shared_ptr<spdlog::logger> Log::_coreLogger;
	std::shared_ptr<spdlog::logger> Log::_clientLogger;

	void Log::init(const std::string &appName) {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		_coreLogger = spdlog::stderr_color_mt("[CJ-Vision Core]");
		_coreLogger->set_level(spdlog::level::trace);
		_coreLogger->flush_on(spdlog::level::trace);

		_clientLogger = spdlog::stderr_color_mt(appName);
		_clientLogger->set_level(spdlog::level::trace);
		_clientLogger->flush_on(spdlog::level::trace);
	}
}
#include "CJ_Vision.h"

extern CJ::Application *CJ::createApplication();

int main(int argc, char **argv) {
	/**
	 * @TODO:
	 * Create startup procedures + loggers and handlers
	 */

	CJ::Log::init("[CJ-Vision Client Startup]");

	/**
	 * Create Application, supports only one
	 */
	auto app = CJ::createApplication();
	std::cout << "App Name: " + app->getName() << std::endl;
	CJ::Log::setClientName("[Runtime - " + app->getName() + "]");

	/**
	 * Super loop runner
	 */
	app->run();

	CJ::Log::setClientName("[Killtime - " + app->getName() + "]");

	/**
	 * App cleanup
	 */
	delete app;
	return 0;
}
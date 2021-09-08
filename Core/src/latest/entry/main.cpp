#include "CJ_Vision.h"

extern CJ::Application *CJ::createApplication();

int main(int argc, char **argv) {
	std::cout << "Test" << std::endl;
	/**
	 * @TODO:
	 * Create startup procedures + loggers and handlers
	 */

	CJ::Log::init("[CJ-Vision Client Startup]");

	/**
	 * Create Application, supports only one
	 */
	auto app = CJ::createApplication();
	CJ::Log::setClientName("[ Runtime - " + app->getName() + "]");

	/**
	 * Super loop runner
	 */
	app->run();


	/**
	 * App cleanup
	 */
	delete app;
	return 0;
}